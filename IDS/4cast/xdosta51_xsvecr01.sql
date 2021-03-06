-- SQL skript pro posledni cast projektu do IDS
-- Zadání č. 26. - Banka

--------------------------------------------------------------------------------

-- Autor: Michal Dostal xdosta51@stud.fit.vutbr.cz
-- Autor: Radek Svec xsvecr01@stud.fit.vutbr.cz

------ SMAZANI TABULEK -------

DROP TABLE operace;
DROP TABLE karta;
DROP TABLE disponence;
DROP TABLE ucet;
DROP TABLE zamestnanec;
DROP TABLE pobocka;
DROP TABLE klient;
DROP TABLE osoba;
DROP SEQUENCE klient_seq;
DROP MATERIALIZED VIEW pocet_operaci;

------ VYTVORENI TABULEK ------

CREATE TABLE osoba (
	rodne_cislo INT PRIMARY KEY,
	jmeno VARCHAR(25) NOT NULL,
	prijmeni VARCHAR(25) NOT NULL,
	email VARCHAR(40) NOT NULL
		CHECK(REGEXP_LIKE(
			email, '^[a-z]+[a-z0-9\.]*@[a-z0-9\.-]+\.[a-z]{2,}$', 'i'
		)),
	telefon VARCHAR(9) NOT NULL
        CHECK(REGEXP_LIKE(
			telefon, '^[0-9]{9}$', 'i'
		)),
	mesto VARCHAR(25) NOT NULL,
	ulice VARCHAR(25) NOT NULL,
	cislo_popisne INT NOT NULL,
	psc INT NOT NULL
        CHECK(REGEXP_LIKE(
			psc, '^[0-9]{5}$', 'i'
		))
);

CREATE TABLE pobocka (
    pobocka_id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    mesto VARCHAR(25) NOT NULL,
    ulice VARCHAR(25) NOT NULL,
	cislo_popisne INT NOT NULL,
	psc INT NOT NULL
        CHECK(REGEXP_LIKE(
			psc, '^[0-9]{5}$', 'i'
		))
);

CREATE TABLE klient (
	klient_id INT DEFAULT NULL PRIMARY KEY,
	datum_registrace DATE DEFAULT CURRENT_TIMESTAMP,
    osoba_id INT NOT NULL,
        CONSTRAINT osoba_klient_id_fk
		FOREIGN KEY (osoba_id) REFERENCES osoba (rodne_cislo)
		ON DELETE CASCADE
);

CREATE TABLE zamestnanec (
    zamestnanec_id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    opravneni VARCHAR(15) NOT NULL,
        CHECK(opravneni IN ('cteni', 'vlastnictvi', 'prispivani', 'upravy')),
    osoba_id INT NOT NULL,
        CONSTRAINT osoba_zamestnanec_id_fk
		FOREIGN KEY (osoba_id) REFERENCES osoba (rodne_cislo)
		ON DELETE CASCADE,
    pobocka_id INT DEFAULT NULL,
        CONSTRAINT zamestnanec_pobocka_id_fk
	    FOREIGN KEY (pobocka_id) REFERENCES pobocka (pobocka_id)
	    ON DELETE SET NULL    
);

CREATE TABLE ucet (
    ucet_cislo INT NOT NULL PRIMARY KEY,
    zustatek NUMBER DEFAULT 0,
    typ VARCHAR(10) NOT NULL
        CHECK(typ IN ('bezny', 'sporici', 'junior')),
    klient_id INT NOT NULL,
        CONSTRAINT vlastnik_ucet_id_fk
		FOREIGN KEY (klient_id) REFERENCES klient (klient_id)
        ON DELETE CASCADE,
    zamestnanec_id INT DEFAULT NULL,
        CONSTRAINT zamestnanec_zridil_id_fk
		FOREIGN KEY (zamestnanec_id) REFERENCES zamestnanec (zamestnanec_id)
        ON DELETE SET NULL
);

CREATE TABLE disponence (
    d_limit VARCHAR(30) DEFAULT 10000,
    opravneni VARCHAR(40) DEFAULT 'zobrazit_zustatek',
	klient_id INT NOT NULL,
        CONSTRAINT klient_id_fk
		FOREIGN KEY (klient_id) REFERENCES klient (klient_id)
        ON DELETE CASCADE,
    ucet_cislo INT NOT NULL,    
        CONSTRAINT ucet_cislo_fk
		FOREIGN KEY (ucet_cislo) REFERENCES ucet (ucet_cislo)
        ON DELETE CASCADE,
    CONSTRAINT disponence_pk
		PRIMARY KEY (klient_id, ucet_cislo)
);

CREATE TABLE karta (
    cislo_karty INT NOT NULL PRIMARY KEY,
	typ_karty VARCHAR(10) NOT NULL,
        CHECK(typ_karty IN ('zlata', 'stribrna', 'debit', 'kredit')),
    klient_id INT NOT NULL,
        CONSTRAINT klient_karta_id_fk
		FOREIGN KEY (klient_id) REFERENCES klient (klient_id)
		ON DELETE CASCADE,
    vlastnici_ucet INT NOT NULL,
        CONSTRAINT vlastnici_ucet_fk
		FOREIGN KEY (vlastnici_ucet) REFERENCES ucet (ucet_cislo)
		ON DELETE CASCADE
);

CREATE TABLE operace (
    operace_id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    datum_provedeni DATE DEFAULT CURRENT_TIMESTAMP,
    typ VARCHAR(20) NOT NULL,
    castka NUMBER,
    stav VARCHAR(15) default 'cekajici'
		CHECK(stav IN ('cekajici', 'dokoncena', 'zrusena', 'probihajici')),
    zamestnanec_id INT DEFAULT NULL,
        CONSTRAINT zamestnanec_provedl_id_fk
		FOREIGN KEY (zamestnanec_id) REFERENCES zamestnanec (zamestnanec_id)
        ON DELETE SET NULL,
    klient_id INT NOT NULL,
        CONSTRAINT klient_operace_id_fk
		FOREIGN KEY (klient_id) REFERENCES klient (klient_id)
		ON DELETE CASCADE,
    ucet_z INT DEFAULT NULL,
        CONSTRAINT ucet_z_fk
		FOREIGN KEY (ucet_z) REFERENCES ucet (ucet_cislo)
		ON DELETE SET NULL,
    ucet_na INT DEFAULT NULL,
        CONSTRAINT ucet_na_fk
		FOREIGN KEY (ucet_na) REFERENCES ucet (ucet_cislo)
		ON DELETE SET NULL
);

------------------------------
--TRIGGERY
------------------------------

/*
Trigger pro prirazovani ID klienta pokud neni zadane pri vytvareni.
*/
CREATE SEQUENCE klient_seq;

CREATE OR REPLACE TRIGGER klient_increment BEFORE
    INSERT ON klient
    FOR EACH ROW
    WHEN ( new.klient_id IS NULL )
BEGIN
    :new.klient_id := klient_seq.nextval;
END;
/

/*
Pri provadeni operace kontroluje jestli neni castka vetsi nez zustatek na uctu z ktereho je provedena a pricte nebo odecte penize z uctu.
*/
CREATE OR REPLACE TRIGGER kontrola_zustatku 
    BEFORE INSERT OR UPDATE OF ucet_z, castka ON operace
    FOR EACH ROW
DECLARE max_platba NUMBER;
BEGIN
    SELECT zustatek INTO max_platba FROM ucet WHERE ucet.ucet_cislo =:new.ucet_z;
    IF (:new.castka > max_platba ) THEN
        raise_application_error(-20111, 'Platba presahuje zustatek na uctu');
    ELSIF (:new.stav != 'zrusena') THEN
        UPDATE ucet SET zustatek = zustatek - :new.castka WHERE ucet.ucet_cislo =:new.ucet_z;
        UPDATE ucet SET zustatek = zustatek + :new.castka WHERE ucet.ucet_cislo =:new.ucet_na;
    END IF;
END;
/

------------------------------
--PROCEDURY
------------------------------

-- procedura pripocteni uroku na sporici ucty
/*
Procedura ma argument procento, ktery udava urokovou sazbu na sporici ucty. 
Vypise stav uctu pred zurocenim a po zuroceni, celkovou castku na vsech sporicich uctech a celkovy urok.
*/
CREATE OR REPLACE PROCEDURE pripocti_urok (procento IN NUMBER) AS 
    c_typ ucet.typ%TYPE;
    c_cislo ucet.ucet_cislo%TYPE;
    c_zustatek ucet.zustatek%TYPE;
    pocet_sporicich INT;
    urok_celkem NUMBER;
    castka_celkem NUMBER;
    CURSOR c_ucet IS SELECT ucet_cislo, typ, zustatek FROM ucet;
BEGIN
    SELECT COUNT(*) INTO pocet_sporicich FROM ucet WHERE typ = 'sporici';
    castka_celkem := 0;
    urok_celkem := 0;
    OPEN c_ucet;
    LOOP
        FETCH c_ucet INTO c_cislo, c_typ, c_zustatek;
        EXIT WHEN c_ucet%notfound;
        IF ( c_typ = 'sporici' ) THEN
            castka_celkem := castka_celkem + c_zustatek;
            urok_celkem := urok_celkem + (c_zustatek * (procento / 100));
            dbms_output.put_line('Zustatek na uctu ' || c_cislo || ' je ' || c_zustatek || 'Kc');
            dbms_output.put_line('Pripocteny urok je ' || (c_zustatek * (procento / 100)) || 'Kc');
            dbms_output.put_line('Zustatek po pricteni uroku je ' || (c_zustatek + (c_zustatek * (procento / 100))) || 'Kc');
            dbms_output.put_line('----------------------------------------------------------------');
            UPDATE ucet SET zustatek = (c_zustatek + (c_zustatek * (procento / 100))) WHERE ucet.ucet_cislo = c_cislo;
        END IF;    
    END LOOP;
    dbms_output.put_line('Zurocene penize ' || castka_celkem || ' Kc ');
    dbms_output.put_line('Celkovy urok ' || urok_celkem || ' Kc ');
    dbms_output.put_line('Pocet sporicich uctu ' || pocet_sporicich);
    CLOSE c_ucet;
END;
/

--vypise utratu na ucte
/*
Procedura zobraz_utratu ma argument cislo_uctu ktery udava ucet, ktereho utratu chceme zobrazit.
Vypise se celkova utrata z uctu a kolik utratil jednotlivy klient a procentualni utratu klienta.
*/
CREATE OR REPLACE PROCEDURE zobraz_utratu (cislo_uctu IN INT) AS 
    c_opid operace.operace_id%TYPE;
    c_castka operace.castka%TYPE;
    c_ucet_z operace.ucet_z%TYPE;
    klient_oper operace.klient_id%TYPE;
    CURSOR c_operace IS SELECT castka, ucet_z, klient_id FROM operace;

    CURSOR c_klient IS SELECT klient_id FROM klient;
    c_klient_id klient.klient_id%TYPE;

    soucet NUMBER;
    celkova_utrata NUMBER;
    procento NUMBER;
BEGIN
    celkova_utrata := 0;
    procento := 0;
    soucet := 0;
    OPEN c_operace;
    LOOP
        FETCH c_operace INTO  c_castka, c_ucet_z, klient_oper;
        EXIT WHEN c_operace%notfound;
        IF (c_ucet_z = cislo_uctu) THEN
            soucet := soucet + c_castka;
        END IF;
    END LOOP;
    dbms_output.put_line('Celkova utrata uctu ' || cislo_uctu || ' je ' || (soucet/soucet) * soucet || ' Kc ');
    CLOSE c_operace;

    OPEN c_klient;
    LOOP
        FETCH c_klient INTO c_klient_id;
        EXIT WHEN c_klient%notfound;
        OPEN c_operace;
        LOOP
            FETCH c_operace INTO  c_castka, c_ucet_z, klient_oper;
            EXIT WHEN c_operace%notfound;
            IF (c_klient_id = klient_oper) THEN
                IF (c_ucet_z = cislo_uctu) THEN
                    celkova_utrata := celkova_utrata + c_castka;
                END IF;
            END IF;
        END LOOP;
        CLOSE c_operace;
        IF (celkova_utrata != 0) THEN
            dbms_output.put_line('Klient ' || c_klient_id || ' utratil ' || celkova_utrata || ' Kc, coz je '  || ROUND((celkova_utrata*100)/soucet, 2) || ' % z celkove utraty');
            celkova_utrata := 0;
        END IF;
    END LOOP;
    CLOSE c_klient;
    
    EXCEPTION WHEN zero_divide THEN
    BEGIN
        dbms_output.put_line('Z uctu nebylo utraceno nic');
    END;
END;
/

-------- VLOZENI DAT DO TABULEK ---------

INSERT INTO osoba (rodne_cislo, jmeno, prijmeni, email, telefon, mesto, ulice, cislo_popisne, psc)
VALUES (9801226655, 'Jan', 'Novak', 'novak@gmail.com', '773821234', 'Brno', 'Hybesova', 211, 76432);
INSERT INTO osoba (rodne_cislo, jmeno, prijmeni, email, telefon, mesto, ulice, cislo_popisne, psc)
VALUES (9902305544, 'Jan', 'Pospech', 'xpospe@gmail.com', '773222111', 'Novy Jicin', 'Ostravska', 356, 75222);
INSERT INTO osoba (rodne_cislo, jmeno, prijmeni, email, telefon, mesto, ulice, cislo_popisne, psc)
VALUES (9305126233, 'Radek', 'Svec', 'svec1@gmail.com', '772333111', 'Kvasice', 'U Zamku', 552, 71233);
INSERT INTO osoba (rodne_cislo, jmeno, prijmeni, email, telefon, mesto, ulice, cislo_popisne, psc)
VALUES (9904186544, 'Petr', 'Fiser', 'petros@gmail.com', '766543211', 'Zlin', 'Jizni Svahy', 1337, 77555);
INSERT INTO osoba (rodne_cislo, jmeno, prijmeni, email, telefon, mesto, ulice, cislo_popisne, psc)
VALUES (8787876555, 'Myspulin', 'Kot', 'kotmysp@gmail.com', '666555444', 'Kocourkov', 'Ocaskova', 13, 10022);
INSERT INTO osoba (rodne_cislo, jmeno, prijmeni, email, telefon, mesto, ulice, cislo_popisne, psc)
VALUES (9235419865, 'Pamela', 'Andersonova', 'AnderPam@gmail.com', '777888999', 'Brno', 'Luxusni', 25, 65487);


INSERT INTO pobocka (mesto, ulice, cislo_popisne, psc)
VALUES ('Zlin', 'Svermova', 3251, 76302);
INSERT INTO pobocka (mesto, ulice, cislo_popisne, psc)
VALUES ('Brno', 'Veveri', 1234, 62500);

INSERT INTO klient (datum_registrace, osoba_id)
VALUES (TO_DATE('1972-11-30', 'yyyy/mm/dd'), 9801226655);
INSERT INTO klient (datum_registrace, osoba_id)
VALUES (TO_DATE('1999-04-28', 'yyyy/mm/dd'), 9902305544);
INSERT INTO klient (datum_registrace, osoba_id)
VALUES (TO_DATE('2005-07-16', 'yyyy/mm/dd'), 9904186544);
INSERT INTO klient (datum_registrace, osoba_id)
VALUES (TO_DATE('2013-04-13', 'yyyy/mm/dd'), 8787876555);
INSERT INTO klient (datum_registrace, osoba_id)
VALUES (TO_DATE('2020-01-08', 'yyyy/mm/dd'), 9235419865);


INSERT INTO zamestnanec (opravneni, osoba_id, pobocka_id)
VALUES ('cteni', 9801226655, 1);
INSERT INTO zamestnanec (opravneni, osoba_id, pobocka_id)
VALUES ('upravy', 9902305544, 2);
INSERT INTO zamestnanec (opravneni, osoba_id, pobocka_id)
VALUES ('vlastnictvi', 9305126233, 2);

INSERT INTO ucet (ucet_cislo, zustatek, typ, klient_id, zamestnanec_id)
VALUES (12345, 566321, 'bezny',  2, 1);
INSERT INTO ucet (ucet_cislo, zustatek, typ, klient_id, zamestnanec_id)
VALUES (13370, 2500666, 'sporici',  1, 1);
INSERT INTO ucet (ucet_cislo, zustatek, typ, klient_id, zamestnanec_id)
VALUES (12346, 566321, 'bezny',  2, 1);
INSERT INTO ucet (ucet_cislo, zustatek, typ, klient_id, zamestnanec_id)
VALUES (11111, 123654, 'bezny',  3, 3);
INSERT INTO ucet (ucet_cislo, zustatek, typ, klient_id, zamestnanec_id)
VALUES (22222, 99666, 'bezny',  4, 3);
INSERT INTO ucet (ucet_cislo, zustatek, typ, klient_id, zamestnanec_id)
VALUES (33333, 99777, 'sporici',  4, 2);



INSERT INTO disponence (d_limit, opravneni, klient_id, ucet_cislo)
VALUES ('5000', 'vyber', 1, 12346);
INSERT INTO disponence (d_limit, opravneni, klient_id, ucet_cislo)
VALUES ('20000', 'vklad', 2, 13370);
INSERT INTO disponence (d_limit, opravneni, klient_id, ucet_cislo)
VALUES ('10000', 'vyber', 5, 11111);
INSERT INTO disponence (d_limit, opravneni, klient_id, ucet_cislo)
VALUES ('4500', 'vyber', 5, 22222);
INSERT INTO disponence (d_limit, opravneni, klient_id, ucet_cislo)
VALUES ('400', 'vyber', 3, 22222);
INSERT INTO disponence (d_limit, opravneni, klient_id, ucet_cislo)
VALUES ('2500', 'vklad', 1, 22222);

INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('zlata', 47791333, 1,12345);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('stribrna', 47791334, 2,12345);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('kredit', 47794001, 1,13370);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('debit', 47794002, 2, 12346);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('stribrna', 11100111, 3, 11111);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('debit', 11100112, 5, 11111);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('debit', 22200222, 4, 22222);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('debit', 22200223, 5, 22222);
INSERT INTO karta (typ_karty, cislo_karty, klient_id, vlastnici_ucet)
VALUES ('stribrna', 22200224, 5, 22222);

INSERT INTO operace (typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('platba', 2, 1, 12345, 13370, 666);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('probihajici', 'platba', 1, 1, 12346, 12345, 253);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('dokoncena', 'platba', 3, 1, 12346, 12345, 333);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('probihajici', 'platba', 2, 2, 11111, 22222, 444);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('zrusena', 'platba', 3, 4, 22222, 12345, 1563);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('dokoncena', 'platba', 3, 4, 22222, 11111, 5541);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('probihajici', 'platba', 3, 3, 22222, 12346, 123);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('cekajici', 'platba', 2, 1, 12345, 22222, 6498);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('dokoncena', 'platba', 2, 1, 12345, 13370, 2511);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('dokoncena', 'platba', 1, 4, 33333, 22222, 756);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('zrusena', 'platba', 3, 1, 22222, 12345, 388);
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('zrusena', 'platba', 3, 5, 22222, 12345, 456);

--pridani noveho zamestnance
INSERT INTO zamestnanec (opravneni, osoba_id, pobocka_id)
VALUES ('upravy', (SELECT rodne_cislo FROM osoba WHERE jmeno = 'Pamela' AND prijmeni = 'Andersonova'), (SELECT pobocka_id FROM pobocka WHERE mesto = 'Brno' AND cislo_popisne = '1234'));

--operace
INSERT INTO operace (stav, typ, klient_id, zamestnanec_id, ucet_z, ucet_na, castka)
VALUES ('cekajici', 'platba', (SELECT klient_id FROM klient WHERE osoba_id = (SELECT rodne_cislo FROM osoba WHERE jmeno = 'Myspulin' AND prijmeni = 'Kot')), (SELECT zamestnanec_id FROM zamestnanec WHERE osoba_id = (SELECT rodne_cislo FROM osoba WHERE jmeno = 'Pamela' AND prijmeni = 'Andersonova')), 22222, 12345, 1563);

------------------------------
--SELECT SPOJENIM DVOU TABULEK
------------------------------
/*
--vypis klientu a jejich zakladni informace
SELECT klient_id, datum_registrace, osoba.rodne_cislo, jmeno, prijmeni, email, telefon, mesto
FROM klient
JOIN osoba ON osoba.rodne_cislo = klient.osoba_id;

--vypis zamestnancu pracujicich na pobocce v brne a jejich zakladni informace
SELECT zamestnanec_id, opravneni, jmeno, prijmeni, email, telefon
FROM zamestnanec
JOIN osoba ON osoba.rodne_cislo = zamestnanec.osoba_id
WHERE pobocka_id = (SELECT pobocka_id FROM pobocka WHERE mesto = 'Brno' AND cislo_popisne = '1234');

--vypis zlatych nebo stribrnych karet a jejich uctu
SELECT vlastnici_ucet, ucet.zustatek, ucet.typ, cislo_karty, typ_karty
FROM ucet
JOIN karta ON ucet.ucet_cislo = vlastnici_ucet
WHERE typ_karty = 'stribrna' OR typ_karty = 'zlata';


------------------------------
--SELECT SPOJENIM TRI TABULEK
------------------------------

--vypis disponentu u uctu s cislem 22222
SELECT disponence.klient_id AS dis_id, jmeno AS dis_jmeno, prijmeni AS dis_prijmeni, opravneni, d_limit AS dis_limit
FROM disponence 
JOIN klient ON klient.klient_id = disponence.klient_id
JOIN osoba ON osoba.rodne_cislo = klient.osoba_id
WHERE disponence.ucet_cislo = '22222';

------------------------------
--GROUP BY S AGREGACNI FUNKCI
------------------------------

--vypis uctu s vice nez jednou kartou a jejich pocet
SELECT ucet.klient_id AS vlastnik_uctu_id, ucet_cislo, ucet.typ, COUNT(vlastnici_ucet) pocet_karet
FROM karta
JOIN ucet ON ucet.ucet_cislo = karta.vlastnici_ucet
GROUP BY karta.vlastnici_ucet, ucet_cislo, ucet.typ, ucet.klient_id
HAVING COUNT(vlastnici_ucet) > 1
ORDER BY ucet.klient_id;


--vypis maximalni a minimalni nezrusene platby ze vsech uctu
SELECT ucet.klient_id, operace.ucet_z, MIN(CAST(castka AS INT)) AS nenizsi_platba, MAX(CAST(castka AS INT)) AS nejvyssi_platba
FROM ucet
JOIN operace ON ucet.ucet_cislo = operace.ucet_z
WHERE NOT operace.stav = 'zrusena'
GROUP BY operace.ucet_z, ucet.klient_id;

-----------
--EXSITS
-----------
--vypise ucty, ktere maji nejake disponenty

SELECT ucet.ucet_cislo as ucty_s_disponenty
FROM ucet
WHERE EXISTS (SELECT disponence.ucet_cislo FROM disponence WHERE disponence.ucet_cislo = ucet.ucet_cislo);


-------------------------
--IN SE VNORENYM SELECTEM
-------------------------
--vypise vsechny registrovane osoby, ktere sdili stejne mesto s pobockou
SELECT * 
FROM osoba
WHERE osoba.mesto IN (SELECT mesto FROM pobocka);
*/

------- UKAZKY TRIGGERU --------------

/*
Ukazka triggeru pro automaticke prirazeni ID.
*/
SELECT klient_id, datum_registrace, osoba_id
FROM klient
ORDER BY klient_id;

--zpusobi trigger prekroceni zustatku na uctu:
INSERT INTO operace (stav, typ, zamestnanec_id, klient_id, ucet_z, ucet_na, castka)
VALUES ('dokoncena', 'platba', 1, 4, 33333, 22222, 1000000);


/* Materialized View */
/*
Vypise informace o jednotlivych uctech a pocet z nich provedenych operaci.
Informace ziska z tabulek druheho clena tymu
*/
CREATE MATERIALIZED VIEW pocet_operaci AS
SELECT
	u.ucet_cislo,
	u.typ,
	u.klient_id,
	COUNT(t.ucet_z) AS pocet_operaci
FROM xsvecr01.ucet u 
LEFT JOIN xsvecr01.operace t ON t.ucet_z = u.ucet_cislo
GROUP BY u.ucet_cislo, u.typ, u.klient_id;

/*
Ukazka vytvoreneho materializovaneho pohledu
*/
SELECT * FROM pocet_operaci;

/*
Smazani 3 operaci z tabulky druheho clena tymu
*/
DELETE FROM xsvecr01.operace WHERE operace_id = 1;
DELETE FROM xsvecr01.operace WHERE operace_id = 2;
DELETE FROM xsvecr01.operace WHERE operace_id = 3;
/*
Materialized view se po smazani operaci nezmenil:
*/
SELECT * FROM pocet_operaci;

/*
Udeleni prav k tabulkam, proceduram a k vytvorenym pohledum.
*/
GRANT ALL ON operace TO xsvecr01;
GRANT ALL ON karta TO xsvecr01;
GRANT ALL ON disponence TO xsvecr01;
GRANT ALL ON ucet TO xsvecr01;
GRANT ALL ON zamestnanec TO xsvecr01;
GRANT ALL ON pobocka TO xsvecr01;
GRANT ALL ON klient TO xsvecr01;
GRANT ALL ON osoba TO xsvecr01;

GRANT EXECUTE ON pripocti_urok TO xsvecr01;
GRANT EXECUTE ON zobraz_utratu TO xsvecr01;
GRANT ALL ON pocet_operaci TO xsvecr01;

GRANT ALL ON operace TO xdosta51;
GRANT ALL ON karta TO xdosta51;
GRANT ALL ON disponence TO xdosta51;
GRANT ALL ON ucet TO xdosta51;
GRANT ALL ON zamestnanec TO xdosta51;
GRANT ALL ON pobocka TO xdosta51;
GRANT ALL ON klient TO xdosta51;
GRANT ALL ON osoba TO xdosta51;

GRANT EXECUTE ON pripocti_urok TO xdosta51;
GRANT EXECUTE ON zobraz_utratu TO xdosta51;
GRANT ALL ON pocet_operaci TO xdosta51;


------- EXPLAIN PLAN -------------------

/*
Explain plan pro vypis uctu s vice nez jednou kartou a jejich pocet
*/
EXPLAIN PLAN FOR
SELECT ucet.klient_id AS vlastnik_uctu_id, ucet_cislo, ucet.typ, COUNT(vlastnici_ucet) pocet_karet
FROM karta
JOIN ucet ON ucet.ucet_cislo = karta.vlastnici_ucet
GROUP BY karta.vlastnici_ucet, ucet_cislo, ucet.typ, ucet.klient_id
HAVING COUNT(vlastnici_ucet) > 1
ORDER BY ucet.klient_id;
SELECT * FROM TABLE(dbms_xplan.display);
/*
Novy index pro tabulku karta, je serazen podle vlasnticiho uctu.
Karty pro stejny ucet jsou hned pod sebou.
*/
CREATE INDEX karta_ucet ON karta (vlastnici_ucet);

/*
S novym indexem je cost(CPU) mensi o 12.
*/
EXPLAIN PLAN FOR
SELECT ucet.klient_id AS vlastnik_uctu_id, ucet_cislo, ucet.typ, COUNT(vlastnici_ucet) pocet_karet
FROM karta
JOIN ucet ON ucet.ucet_cislo = karta.vlastnici_ucet
GROUP BY karta.vlastnici_ucet, ucet_cislo, ucet.typ, ucet.klient_id
HAVING COUNT(vlastnici_ucet) > 1
ORDER BY ucet.klient_id;
SELECT * FROM TABLE(dbms_xplan.display);


--zavolani procedury pripocti_urok s urokem 14%
BEGIN pripocti_urok(0.14); END;
/
--ukazka procedury zobraz_utratu pro ucet 22222, vypise utratu jednotlivych klientu na spolecnem uctu s cislem 22222
BEGIN zobraz_utratu(22222); END;
/
--vyvola exception divide_zero, protoze z uctu neni nic utraceno
BEGIN zobraz_utratu(13370); END;
/