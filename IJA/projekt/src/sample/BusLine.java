/**
 * Implementace tridy pro autobusove linky
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

import java.util.ArrayList;
import java.util.List;

public class BusLine {

    private String id;
    private List<Stop> stopos = new ArrayList<Stop>();
    private List<Street> streetos = new ArrayList<Street>();

    /**
     * Konstruktor tridy
     * @param idd String jmeno linky
     */
    protected BusLine(String idd) {
        this.id = idd;
    }

    /**
     * Pridava ulici se zastavkou do linky, kontroluje, zda zastavka na ulici lezi
     * @param street Street pridavana ulice
     * @param stop Stop pridavana zastavka
     * @return boolean vraci true, pokud se ulici se zastavkou podarilo pridat, jinak false
     */
    public boolean addStreet(Street street, Stop stop) {
        if (this.streetos.size() == 0 || this.streetos.get(this.streetos.size() - 1).follows(street)) {
            if (stop.getStreet().getId().compareTo(street.getId()) == 0) {
                this.streetos.add(street);
                this.stopos.add(stop);
                return true;
            }
        }
        return false;
    }

    /**
     * Pridava samotnou ulici bez zastavky do linky
     * @param street Street pridavana ulice
     * @return boolean true, pokud se ulici do linky povedlo pridat, jinak false
     */
    public boolean addStreet(Street street) {
        if (this.streetos.size() == 0 || this.streetos.get(this.streetos.size() - 1).follows(street)) {
            this.streetos.add(street);
            return true;
        } else return false;
    }

    /**
     * Vraci vsechny ulice linky
     * @return List Street  seznam ulic linky
     */
    public List<Street> getStreets() {
        return this.streetos;
    }

    /**
     * Vraci vsechny zastavky linky
     * @return List Stop  seznam zastavek linky
     */
    public List<Stop> getStops() {
        return this.stopos;
    }

    /**
     * Vraci jmeno linky
     * @return String jmeno linky
     */
    public String getId() {
        return this.id;
    }

    /**
     * Vytvari seznam souradnic predstavujicich trasu linky
     * @return List Coordinate  seznam souradnic trasy linky
     */
    public List<Coordinate> makeRoute(){
        List<Coordinate> coords = new ArrayList<Coordinate>();
        int i = 0;
        Street prevStre = null;
        for(Street stre : this.streetos){
            if (i == 0) {
                prevStre = stre;
                i++;
                continue;
            }
            if (prevStre.end().equals(stre.begin()) || prevStre.end().equals(stre.end())) {
                for (Coordinate c : prevStre.getCoordinates()) {
                    coords.add(c);
                }
            } else if (prevStre.begin().equals(stre.begin()) || prevStre.begin().equals(stre.end())) {
                List<Coordinate> reversed = new ArrayList<Coordinate>();
                for (Coordinate c : prevStre.getCoordinates()) {
                    reversed.add(0, c);
                }
                for (Coordinate c : reversed) {
                    coords.add(c);
                }
            }
            prevStre = stre;
        }

        if(prevStre.end().equals(coords.get(0)) || prevStre.end().equals(coords.get(0))){
            for (Coordinate c : prevStre.getCoordinates()) {
                coords.add(c);
            }
        }
        else if(prevStre.begin().equals(coords.get(0)) || prevStre.begin().equals(coords.get(0))){
            List<Coordinate> reversed = new ArrayList<Coordinate>();
            for (Coordinate c : prevStre.getCoordinates()) {
                reversed.add(0, c);
            }
            for (Coordinate c : reversed) {
                coords.add(c);
            }
        }

        List<Coordinate> nodup = new ArrayList<Coordinate>();
        for(Coordinate c :coords){
            int isin = 0;
            for (Coordinate cn : nodup) {
                if (c.equals(cn)) {
                    isin = 1;
                }
            }
            if (isin == 0) {
                nodup.add(c);
            }
        }
        Coordinate first = nodup.get(0);
        nodup.remove(0);
        nodup.add(first);
        nodup.add(0, this.stopos.get(0).getCoordinate());
        nodup.add(this.stopos.get(0).getCoordinate());
        return nodup;
    }
}
