/**
 * Coordinate - trida pro praci se souradnicemi
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

public class Coordinate{

    private double x = 0;
    private double y = 0;

    /**
     * Konstruktor pro tridu Coordinate
     * @param cx double bod souradnice na ose X
     * @param cy double bod souradnice na ose Y
     */
    protected Coordinate(double cx, double cy){
        this.x = cx;
        this.y = cy;
    }

    /**
     * 
     * @return double vraci bod souradince na ose X
     */
    public double getX(){
        return this.x;
    }

    /**
     * 
     * @return double vraci bod souradnice na ose Y
     */
    public double getY(){
        return this.y;
    }

    /**
     * Vypocita vzdalenost na ose X mezi 2 Coordinaty
     * @param c Coordinate porovnavana souradnice Coordinate
     * @return double vraci vzdalenost na ose X
     */
    public double diffX(Coordinate c){
        return this.x - c.getX();
    }

    /**
     * Vypocita vzdalenost na ose Y mezi 2 Coordinaty
     * @param c Coordinate porovnavana souradnice Coordinate
     * @return double vraci vzdalenost na ose Y
     */
    public double diffY(Coordinate c){
        return this.y - c.getY();
    }

    /**
     * Porovna dva objekty Coordinate jestli se rovnaji
     * @param obj Object porovnavany objekt Coordinate
     * @return boolean vraci true pokud se souradnice rovnaji, jinak false
     */
    public boolean equals(Object obj){
        if(obj == this) {
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        Coordinate cordo = (Coordinate) obj;
        if(this.x == cordo.getX() && this.y == cordo.getY()){
            return true;
        }
        else return false;
    }

    /**
     * Override pro hashCode
     * @return int vraci vypocitany hash code
     */
    public int hashCode() {
        double result = 17;
        result = 31 * result + x;
        result = 31 * result + y;
        return (int)result;
    }

    /**
     * Funkce pro vytvoreni nove souradnice se zadanymi body X a Y
     * @param cx double bod na ose X vytvarene souradnice
     * @param cy double bod na ose Y vytvarene souradnice
     * @return Coordinate vraci vytvoreny objekt
     */
    public static Coordinate create(double cx, double cy) {
        if(cx >= 0 && cy >= 0){
            return new Coordinate(cx, cy);
        }
        else{
            return null;
        }
    }
}