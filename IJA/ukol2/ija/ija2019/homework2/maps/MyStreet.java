/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package ija.ija2019.homework2.maps;

import java.util.ArrayList;
import java.util.List;

import ija.ija2019.homework2.maps.Street;

public class MyStreet implements Street{
    
    private String id;
    private List<Coordinate> coords = new ArrayList<Coordinate>();
    private List<Stop> stops;
    
    public MyStreet(String i, Coordinate...coordinates){
        this.id = i;
        for(Coordinate c: coordinates){
            this.coords.add(c);
        }
        this.stops = new ArrayList<Stop>();
    }

    public String getId(){
        return this.id;
    }
    
    public List<Coordinate> getCoordinates(){
        return this.coords;
    }
    
    public List<Stop> getStops(){
        return this.stops;
    }
    
    public boolean addStop(Stop stop){
        
        int py = stop.getCoordinate().getY();
        int px = stop.getCoordinate().getX();
        Coordinate c0 = null;
        for(Coordinate c1: coords){
            if(c0 != null && c0 != c1){
                int x1 = c0.getX();
                int y1 = c0.getY();
                
                int x2 = c1.getX();
                int y2 = c1.getY();

                if((x1 >= px && x2 <= px || x1 <=px && x2 >= px) && (y1 >= py && y2 <= py || y1 <=py && y2 >= py)){
                    int[] n = {-(y2-y1), x2-x1};
                    int c = -n[0]*x1 -n[1]*y1;
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

    public Coordinate begin() {
        return this.coords.get(0);
    }

    public Coordinate end() {
        return this.coords.get(this.coords.size() - 1 );
    }

    public boolean follows(Street s) {
        if(this.begin().equals(s.begin()) || this.begin().equals(s.end()) || this.end().equals(s.end()) || this.end().equals(s.begin())){
            return true;
        }
        else return false;
    }
    

}