/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package ija.ija2019.homework2.maps;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.List;
import ija.ija2019.homework2.maps.Line;

public class MyLine implements Line {

    private String id;
    private List<AbstractMap.SimpleImmutableEntry<Street, Stop>> listLine = new ArrayList<AbstractMap.SimpleImmutableEntry<Street,Stop>>();

    protected MyLine(String idd) {
        this.id = idd;
    }

    public boolean addStop(Stop stop) {
        if (this.listLine.size() == 0 || this.listLine.get(this.listLine.size()-1).getKey().follows(stop.getStreet())) {
            this.listLine.add(new AbstractMap.SimpleImmutableEntry<Street, Stop>(stop.getStreet(), stop));
            return true;
        }
        else return false;
    }

    public boolean addStreet(Street street){
        if(this.listLine.size() != 0 || this.listLine.get(this.listLine.size()-1).getKey().follows(street)){
            this.listLine.add(new AbstractMap.SimpleImmutableEntry<Street, Stop>(street, null));
            return true;
        }
        else return false;
    }

    public List<AbstractMap.SimpleImmutableEntry<Street,Stop>> getRoute(){
        return new ArrayList<AbstractMap.SimpleImmutableEntry<Street, Stop>>(this.listLine);
    }
}
