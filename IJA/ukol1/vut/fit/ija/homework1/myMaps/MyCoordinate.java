/**
 *
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 */

package vut.fit.ija.homework1.myMaps;

import vut.fit.ija.homework1.maps.Coordinate;

public class MyCoordinate implements Coordinate{

    private int x = 0;
    private int y = 0;

    public int getX(){
        return this.x;
    }

    public int getY(){
        return this.y;
    }

    private MyCoordinate(int cx, int cy){
        this.x = cx;
        this.y = cy;
    }

    public static MyCoordinate create(int cx, int cy){
        if(cx >= 0 && cy >= 0){
            return new MyCoordinate(cx, cy);
        }
        else{
            return null;
        }
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
}