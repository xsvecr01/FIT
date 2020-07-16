/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package vut.fit.ija.homework1.myMaps;

import java.util.ArrayList;
import java.util.List;

import vut.fit.ija.homework1.maps.Coordinate;
import vut.fit.ija.homework1.maps.Stop;
import vut.fit.ija.homework1.maps.Street;

public class MyStreet implements Street{
    
    private String id;
    private MyCoordinate c1;
    private MyCoordinate c2;
    private List<Stop> stops;
    
    public MyStreet(String i, MyCoordinate cc1, MyCoordinate cc2){
        this.id = i;
        this.c1 = cc1;
        this.c2 = cc2;
        this.stops = new ArrayList<Stop>();
    }

    public String getId(){
        return this.id;
    }
    
    public List<Coordinate> getCoordinates(){
        List<Coordinate> l1 = new ArrayList<Coordinate>();
        l1.add(this.c1);
        l1.add(this.c2);
        return l1;
    }
    
    public List<Stop> getStops(){
        return this.stops;
    }
    
    public void addStop(Stop stop){
        stop.setStreet(this);
        this.stops.add(stop);
    }
}