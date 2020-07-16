/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package ija.ija2019.homework2.maps;

import ija.ija2019.homework2.maps.Coordinate;

public class Coordinate{

    private int x = 0;
    private int y = 0;

    protected Coordinate(int cx, int cy){
        this.x = cx;
        this.y = cy;
    }

    public int getX(){
        return this.x;
    }

    public int getY(){
        return this.y;
    }

    public int diffX(Coordinate c){
        return this.x - c.getX();
    }

    public int diffY(Coordinate c){
        return this.y - c.getY();
    }

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

    public int hashCode() {
        int result = 17;
        result = 31 * result + x;
        result = 31 * result + y;
        return result;
    }

    public static Coordinate create(int cx, int cy) {
        if(cx >= 0 && cy >= 0){
            return new Coordinate(cx, cy);
        }
        else{
            return null;
        }
    }
}