/**
 * Interface pro casovou aktualizaci
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

import java.time.LocalTime;

public interface TimeUpdate {
    void update(LocalTime time);
}
