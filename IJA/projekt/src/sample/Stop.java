/**
 * Trida pro zastavky
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

public class Stop {
    private String id;
    private Coordinate c1;
    private Street street = null;

    /**
     * Konstruktor pro vytvareni zastavky
     * @param i String jmeno zastavky
     * @param cc1 Coordinate souradnice zastavky
     */
    protected Stop(String i, Coordinate cc1){
        this.id = i;
        this.c1 = cc1;
    }

    /**
     * Pretezovani konstruktoru pro vytvareni zastavky pouze s id
     * @param i String jmeno zastavky
     */
    protected Stop(String i){
        this.id = i;
        this.c1 = null;
    }

    /**
     * Navraceni jmena zastavky
     * @return String jmeno zastavky
     */
    public String getId(){
        return this.id;
    }

    /**
     * Vraci souradnice zastavky
     * @return Coordinate souradnice zastavky
     */
    public Coordinate getCoordinate(){
        return this.c1;
    }

    /**
     * Prida zastavku do ulice
     * @param s Street ulice do ktere se zastavka pridava
     */
    public void setStreet(Street s){
        this.street = s;
    }

    /**
     * Vraci ulici na ktere zastavka lezi
     * @return Street ulice na ktere je zastavka
     */
    public Street getStreet(){
        return this.street;
    }

    /**
     * Override metody equals - porovnava dve zastavky, jestli jsou stejne. Porovnavaji se podle id.
     * @param obj Object porovnavana zastavka
     * @return boolean vraci true, pokud se rovnaji, jinak false
     */
    public boolean equals(Object obj){
        if(obj == this) {
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        Stop stoppo = (Stop) obj;
        if(this.id.compareTo(stoppo.getId()) == 0){
            return true;
        }
        else return false;
    }

    /**
     * Override metody hashCode - vypocitava hash
     * @return int vraci vypocitanou hash hodnotu
     */
    public int hashCode() {
        int result = 17;
        result = 31 * result + id.hashCode();
        result = 31 * result + c1.hashCode();
        result = 31 * result + street.hashCode();
        return result;
    }
}
