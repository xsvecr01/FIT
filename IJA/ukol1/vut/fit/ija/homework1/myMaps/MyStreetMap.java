/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package vut.fit.ija.homework1.myMaps;

import java.util.ArrayList;
import java.util.List;

import vut.fit.ija.homework1.maps.Street;
import vut.fit.ija.homework1.maps.StreetMap;

public class MyStreetMap implements StreetMap{

    private List<Street> streets;

    public MyStreetMap(){
        this.streets = new ArrayList<Street>();
    }
    
    public void addStreet(Street s){
        this.streets.add(s);
    }
    
    public Street getStreet(String id){
        for(Street stret : streets){
            if(stret.getId().equals(id)){
                return stret;
            }
        }
        return null;
    }
}