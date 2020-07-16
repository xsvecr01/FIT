/**
 * Implementace tridy pro vozidla na mape
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Shape;

import java.time.LocalTime;
import java.util.ArrayList;
import java.util.List;

public class Vehicle implements Drawable, TimeUpdate {
    public Coordinate position;
    public double speed;
    private double distance = 0;
    private Path path;
    private List<Shape> gui;
    double pathSize;
    private BusLine busline;
    public String buslineId;
    private Color coloris =  Color.BLUE;
    private Circle circle;
    public Stop lastStop = new Stop("", Coordinate.create(0,0));
    public Stop tempStop = new Stop("", Coordinate.create(0,0));
    public LocalTime stopTime = LocalTime.parse("00:00:00");

    /**
     * Konstruktor pro objekt Vehicle 
     * @param speed double rychlost vozidla
     * @param busline BusLine trasa, po ktere vozidlo bude jezdit (linka)
     */
    public Vehicle(double speed, BusLine busline) {
        this.position = busline.getStops().get(0).getCoordinate();
        this.path = new Path(busline.makeRoute());
        this.buslineId = busline.getId();
        this.busline = busline;
        this.tempStop = busline.getStops().get(0);

        this.speed = speed;
        gui = new ArrayList<>();
        circle = new Circle(position.getX(), position.getY(), 4, coloris);
        gui.add(circle);
        pathSize = path.getPathSize();
    }

    /**
     * Vyresetovani ujete vzdalenosti a obnoveni cesty
     */
    public void reset(){
        distance = 0;
        path = new Path(busline.makeRoute());
    }

    /**
     * Nastaveni barvy objektu
     * @param color Color pozadovana barva
     */
    public void setColor(Color color){
        circle.setFill(color);
        this.coloris = color;
    }

    /**
     * Vypocitava posun vozidla
     * @param coordinate Coordinate souradnice na kterou se ma vozidlo posunovat
     */
    private void nodeGui(Coordinate coordinate) {
        for (Shape shape : gui) {
            shape.setTranslateX((coordinate.getX() - position.getX()) + shape.getTranslateX());
            shape.setTranslateY((coordinate.getY() - position.getY()) + shape.getTranslateY());
        }
    }

    /**
     * Vraci linku, po ktere vozidlo jezdi
     * @return BusLine linka vozidla
     */
    public BusLine getBusline(){
        return  this.busline;
    }

    /**
     * Vraci graficky objekt vozidla
     * @return List Shape list grafickych objektu
     */
    @Override
    public List<Shape> getGUI() {
        return gui;
    }

    /**
     * Aktualizuje pozici vozidla, je zavisla na case
     * Provadi zastavovani na zastavkach
     * @param time LocalTime aktualni cas
     */
    @Override
    public void update(LocalTime time) {
        distance += speed;
        if (distance > pathSize) {
            return;
        }

        Coordinate coords;
        int i = 0;
        if(i == 0){
            i++;
            coords = position;
        }
        coords = path.getCoordinatesbyDistance(distance);

        nodeGui(coords);
        position = coords;

        for(Stop s : busline.getStops()){
            if(s.getCoordinate().diffX(position) <= 2 && s.getCoordinate().diffX(position) >= -2 &&
                s.getCoordinate().diffY(position) <= 2 && s.getCoordinate().diffY(position) >= -2){
                lastStop = s;
                if(!tempStop.equals(lastStop)){
                    stopTime = time;
                    tempStop = lastStop;
                    speed = 0;
                }
            }
        }
    }
}
