/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package ija.ija2019.homework2.maps;

import java.util.AbstractMap;
import java.util.List;

public interface Line {

    public static Line defaultLine(String id){
        return new MyLine(id);
    }

    public boolean addStop(Stop stop);

    public boolean addStreet(Street street);

    public List<AbstractMap.SimpleImmutableEntry<Street,Stop>> getRoute();
    
}
