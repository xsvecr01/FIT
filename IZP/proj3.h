/**
 * @mainpage Projekt 3 - dokumentace
 * 
 * @file Proj3.h
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2018/19
 * @brief Projekt 3: Jednoducha shlukova analyza - 2D nejblizsi soused
 *
 */

/**
 * @defgroup clusters Prace se shluky
 * @{
 */

/**
 * @brief Struktura objektu
 * 
 */
struct obj_t {
    /**ID objektu*/
    int id;
    /**X souradnice*/
    float x;
    /**Y souradnice*/
    float y;
};

/**
 * @brief Struktura shluku objektu
 * 
 */
struct cluster_t {
    /**Pocet objektu ve shluku*/
    int size; 
    /**Kapacitu shluku (pocet objektu, pro ktere je rezervovano misto v poli)*/
    int capacity; 
    /**Ukazatel na pole shluku*/
    struct obj_t *obj;
};

/**
 * @brief Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 * Ukazatel NULL u pole objektu znamena kapacitu 0.
 * 
 * @post Shluk 'c' bude mit alokovanou pamet pro 'cap' objektu
 * 
 * @param c Inicializovany shluk
 * @param cap Vysledna kapacita inicializovaneho shluku
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief  Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 * 
 * @post Alokovana pamet pro objekty shluku 'c' bude uvolnena
 * 
 * @param c Shluk, ktery ma byt uvolnen
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief Chunk objektu shluku, hodnota doporucena pro realokaci.
 * 
 */
extern const int CLUSTER_CHUNK;

/**
 * @brief  Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * 
 * @pre Kapacita shluku musi byt vetsi nebo rovna 0
 * 
 * @post Shluk 'c' bude mit nastavenou kapacitu na hodnotu 'new_cap'
 * 
 * @param c Shluk, kde ma byt zmenena kapacita
 * @param new_cap Nova kapacita shluku
 * @return Ukazatel na vysledny shluk
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt nevejde.
 * 
 * @post Objekt 'obj' bude poslednim objektem shluku 'c'
 * 
 * @param c Shluk, do ktereho je pridavan objekt
 * @param obj Objekt pridavany do shluku
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Do shluku 'c1' prida objekty 'c2'. Pokud je potreba, shluk 'c1' bude rozsiren.
 * 
 * @pre Shluk 'c1' a 'c2' nesmi byt NULL
 * 
 * @post Shluk 'c1' s pridanymi objekty ze shluku 'c2'. Objekty ve shluku 'c1' jsou serazeny podle identifikacniho cisla.
 * Shluk 'c2' zustal nezmenen.
 * 
 * @param c1 Shluk, ke kteremu se budou objekty pridavat.
 * @param c2 Shluk, ze ktereho se objekty vezmou.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'.
 * 
 * @pre 'idx' je mensi nez 'narr'
 * @pre 'narr' je vetsi nez 0
 * 
 * @post Odstrani z pole 'carr' shluk na indexu 'idx'
 * 
 * @param carr Pole shluku
 * @param narr Pocet polozek 
 * @param idx Index shluku pro odstraneni
 * @return Novy pocet shluku v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Vypocita Euklidovskou vzdalenost mezi dvema objekty. 
 * 
 * @pre Objekty 'o1' a 'o2' nesmí být NULL
 * 
 * @param o1 Objekt 1
 * @param o2 Objekt 2
 * @return Vysledna vzdalenost mezi objekty
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Pocita vzdalenost dvou shluku.
 * 
 * @pre Pocet objektu shluku 'c1' a 'c2' je vetsi nez 0
 * 
 * @param c1 Shluk 1
 * @param c2 Shluk 2
 * @return Vzdalenost shluku 'c1' a 'c2'
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'.
 * 
 * @pre Velikost pole 'narr' je vetsi nez 0
 * 
 * @post Nalezene shluky (indexy do pole 'carr') budou ulozeny do pameti na adresy 'c1' a 'c2'
 * 
 * @param carr Pole shluku
 * @param narr Velikost pole
 * @param c1 Index prvniho nalezeneho shluku
 * @param c2 Index druheho nalezeneho shluku
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 * 
 * @post Objekty shluku 'c' budou serazeny vzestupne podle identifikatoru
 * 
 * @param c Shluk, ve kterem maji byt serazeny objekty
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Tisk shluku 'c' na stdout.
 * 
 * @post Vypise objekty shluku 'c' na stdout
 * 
 * @param c Shluk pro tisk
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 * jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 * polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 * kam se odkazuje parametr 'arr'.
 * V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 * 
 * @pre 'arr' nesmi byt NULL
 * @pre Musi existovat soubor 'filename' s pristupovymi pravy a se spravne napsanym obsahem
 * 
 * @post Pro kazdy objekt je vytvoren shluk. Tento shluk je pak ulozen do pole 'arr'. 
 * Pole 'arr' ma alokovanou pamet o velikosti poctu objektu v souboru.
 * 
 * @param filename Soubor s definici objektu
 * @param arr Ukazatel na prvni shluk v alokovanem poli
 * @return Pocet nactenych objektu (shluku)
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Tisk pole shluku.
 * 
 * @post Objekty shluku v poli 'carr' se vypisi na stdout
 * 
 * @param carr ukazatel na prvni polozku (shluk)
 * @param narr Pocet shluku pro tisk
 */
void print_clusters(struct cluster_t *carr, int narr);
/**
 * @}
 */