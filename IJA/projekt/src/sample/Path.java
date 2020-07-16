/**
 * Implementace tridy pro trasu
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

import java.util.List;

public class Path {
    private List<Coordinate> path;

    /**
     * Konstruktor tridy
     * @param path List Coordinate  seznam souradnic
     */
    public Path(List<Coordinate> path) {
        this.path = path;
    }

    /**
     * Vraci vzdalenost mezi dvema souradnicemi
     * @param a Coordinate souradnice A
     * @param b Coordinate souradnice B
     * @return double vypocitana vzdalenost souradnic
     */
    private double getDistanceBetweenCoordinates(Coordinate a, Coordinate b) {
        return Math.sqrt(Math.pow(a.getX() - b.getX(), 2) + Math.pow(a.getY() - b.getY(), 2));
    }

    /**
     * Vypocitava nasledujici pozici vozidla
     * @param distance double ujeta vzdalenost vozidla
     * @return Coordinate nasledujici pozice vozidla
     */
    public Coordinate getCoordinatesbyDistance(double distance) {
        double length = 0;
        double currentLength = 0;

        Coordinate a = null;
        Coordinate b = null;
        for (int i = 0; i < path.size() -1; i++) {
            a = path.get(i);
            b = path.get(i+1);
            currentLength = getDistanceBetweenCoordinates(a, b);
            if (length + getDistanceBetweenCoordinates(a ,b) >= distance) {
                break;
            }
            length += currentLength;

        }
        if (a == null || b == null) {
            return null;
        }
        double driven = (distance - length) / currentLength;
        return new Coordinate(a.getX() + (b.getX() - a.getX()) * driven, a.getY() + (b.getY() - a.getY()) * driven);
    }

    /**
     * Vypocitava delku cele trasy
     * @return double delka trasy
     */
    public double getPathSize() {
        double size = 0;
        int i = 0;
        Coordinate cprev = null;
        for (Coordinate coordinate : path) {
            if(i == 0){
                i++;
                cprev = coordinate;
                continue;
            }
            size += getDistanceBetweenCoordinates(cprev, coordinate);
            cprev = coordinate;
        }
        return size;
    }
}
