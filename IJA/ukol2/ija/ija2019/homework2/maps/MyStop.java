/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package ija.ija2019.homework2.maps;

import ija.ija2019.homework2.maps.Stop;

public class MyStop implements Stop{
    
    private String id;
    private Coordinate c1;
    private Street street = null;

    protected MyStop(String i, Coordinate cc1){
        this.id = i;
        this.c1 = cc1;
    }

    protected MyStop(String i){
        this.id = i;
        this.c1 = null;
    }
    
    public String getId(){
        return this.id;
    }
    
    public Coordinate getCoordinate(){
        return this.c1;
    }

    public void setStreet(Street s){
        this.street = s;
    }

    public Street getStreet(){
        return this.street;
    }

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

    public int hashCode() {
        int result = 17;
        result = 31 * result + id.hashCode();
        result = 31 * result + c1.hashCode();
        result = 31 * result + street.hashCode();
        return result;
    }

    public String toString(){
        return "stop(" + this.getId() + ")";
    }

}