/*
 * Zdrojové kódy josu součástí zadání 1. úkolu pro předmětu IJA v ak. roce 2019/2020.
 * (C) Radek Kočí
 */
package ija.ija2019.homework2.maps;

import java.util.List;

/**
 * Reprezentuje jednu ulici v mapě. Ulice má svůj identifikátor (název) a je definována souřadnicemi. Pro 1. úkol
 * předpokládejte pouze souřadnice začátku a konce ulice.
 * Na ulici se mohou nacházet zastávky.
 * @author koci
 */
public interface Street {
    /**
     * Vrátí identifikátor ulice.
     * @return Identifikátor ulice.
     */
    public String getId();
    
    /**
     * Vrátí seznam souřadnic definujících ulici. První v seznamu je vždy počátek a poslední v seznamu konec ulice.
     * @return Seznam souřadnic ulice.
     */
    
    public List<Coordinate> getCoordinates();
    
    /**
     * Vrátí seznam zastávek na ulici.
     * @return Seznam zastávek na ulici. Pokud ulize nemá žádnou zastávku, je seznam prázdný.
     */
    public List<Stop> getStops();
    
    /**
     * Přidá do seznamu zastávek novou zastávku.
     * @param stop Nově přidávaná zastávka.
     * @return 
     */
    public boolean addStop(Stop stop);

    public Coordinate begin();

    public Coordinate end();

    public boolean follows(Street s);

	public static Street defaultStreet(String id, Coordinate ...coordinates){
        boolean rightangle = true;
        Coordinate c0 = null;
        int[] vector0 = {0,0}, vector1 = {0,0};
        if(coordinates.length != 2){
            for(Coordinate c1: coordinates){
                if(c0 != null){
                    vector1[0] = c1.diffX(c0);
                    vector1[1] = c1.diffY(c0);
                    if(vector0[0] != 0 && vector0[1] != 0){
                        if((vector0[0] * vector1[0] + vector0[1] * vector1[1]) != 0){
                            rightangle = false;
                            return null;
                        }
                    }
                }
                c0 = c1;
                vector0 = vector1;
            }
            if(rightangle){
                return new MyStreet(id, coordinates);
            }
        }
        if(rightangle){
            return new MyStreet(id, coordinates);
        }
        else return null;
    }
}
