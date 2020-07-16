/**
 * Implementace tridy pro objekt ulice
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.shape.Shape;

import java.util.ArrayList;
import java.util.List;

public class Street implements Drawable {
    private String id;
    private List<Coordinate> coords = new ArrayList<Coordinate>();
    private List<Stop> stops = new ArrayList<Stop>();
    private List<Shape> linosos = new ArrayList<Shape>();
    public boolean isSlow = false;

    /**
     * Konstruktor objektu Street
     * @param name String jmeno ulice
     * @param coordinates List Coordinate  list souradnic, kterymi ulice vede
     */
    public Street(String name, List<Coordinate> coordinates){
        this.id = name;
        this.coords = coordinates;
    }

    /**
     * Vytvari graficke objekty pro ulice a pro zastavky
     * @return List Shape  seznam grafickych objektu
     */
    @Override
    public List<Shape> getGUI() {

        Coordinate cprev = null;
        for(Coordinate c: coords) {
            if (cprev != null && c != cprev) {
                Line linos = new Line (cprev.getX(), cprev.getY(), c.getX(), c.getY());
                linos.setStroke(Color.BLACK);
                linosos.add(linos);
            }
            cprev = c;
        }

        for(Stop s : this.stops){
            Circle ciros = new Circle(s.getCoordinate().getX(), s.getCoordinate().getY(), 2, Color.RED);
            ciros.setStroke(Color.BLACK);
            linosos.add(ciros);
        }

        return linosos;
    }

    /**
     * Nastavuje barvu grafickych objektu ulice
     * @param color Color nastavovana barva
     */
    public void setColor(Color color){
        for(Shape shapos: linosos){
            shapos.setStroke(color);
        }
    }

    /**
     * Vraci jmeno ulice
     * @return String jmeno ulice
     */
    public String getId(){
        return this.id;
    }

    /**
     * Vraci list souradnic, kterymi ulice vede
     * @return List Coordinate  list souradnic
     */
    public List<Coordinate> getCoordinates(){
        return this.coords;
    }

    /**
     * Vraci list zastavek, ktere lezi na ulici
     * @return List Stop  list zastavek
     */
    public List<Stop> getStops(){
        return this.stops;
    }

    /**
     * Pridava zastavku na ulici, pri cemz kontroluje, zda zastavka na ulici lezi
     * @param stop Stop pridavana zastavka
     * @return boolean vraci true, pokud zastavka na ulici lezi, jinak false
     */
    public boolean addStop(Stop stop){

        double py = stop.getCoordinate().getY();
        double px = stop.getCoordinate().getX();
        Coordinate c0 = null;
        for(Coordinate c1: coords){
            if(c0 != null && c0 != c1){
                double x1 = c0.getX();
                double y1 = c0.getY();

                double x2 = c1.getX();
                double y2 = c1.getY();

                if((x1 >= px && x2 <= px || x1 <=px && x2 >= px) && (y1 >= py && y2 <= py || y1 <=py && y2 >= py)){
                    double[] n = {-(y2-y1), x2-x1};
                    double c = -n[0]*x1 -n[1]*y1;
                    if((n[0]*px + n[1]*py + c) == 0){
                        stop.setStreet(this);
                        this.stops.add(stop);
                        return true;
                    }
                }
            }
            c0 = c1;
        }
        return false;
    }

    /**
     * Vraci pocatecni souradnici ulice
     * @return Coordinate pocatecni souradnice
     */
    public Coordinate begin() {
        return this.coords.get(0);
    }

    /**
     * Vraci koncovou souradnici ulice
     * @return Coordinate koncova souradnice
     */
    public Coordinate end() {
        return this.coords.get(this.coords.size() - 1 );
    }

    /**
     * Kontroluje, zda na sebe ulice navazuji
     * @param s Street kontrolovana ulice
     * @return boolean pokud na sebe ulice navazuji vraci true, jinak false
     */
    public boolean follows(Street s) {
        if(this.begin().equals(s.begin()) || this.begin().equals(s.end()) || this.end().equals(s.end()) || this.end().equals(s.begin())){
            return true;
        }
        else return false;
    }

    /**
     * S odchylkou kontroluje, zda souradnice lezi na ulici
     * @param point Coordinate porovnavana souradnice
     * @return boolean vraci true, pokud je souradnice v blizkosti ulice, jinak false
     */
    public boolean inLine(Coordinate point) {

        int py = (int)point.getY();
        int px = (int)point.getX();
        Coordinate c0 = null;
        for(Coordinate c1: coords){
            if(c0 != null && c0 != c1){
                int x1 = (int)c0.getX();
                int y1 = (int)c0.getY();

                int x2 = (int)c1.getX();
                int y2 = (int)c1.getY();

                if((x1 >= px && x2 <= px || x1 <=px && x2 >= px) && (y1 >= py && y2 <= py || y1 <=py && y2 >= py)){
                    int[] n = {-(y2-y1), x2-x1};
                    int c = -n[0]*x1 -n[1]*y1;
                    int tempRes = (n[0]*px + n[1]*py + c);
                    if(tempRes < 100 && tempRes > -100){
                        return true;
                    }
                }
            }
            c0 = c1;
        }
        return false;
    }

}
