/**
 * Implementace tridy pro celkove ovladani aplikace
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */

package sample;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class Controller {

    public List<Stop> allStops = new ArrayList<Stop>();
    public List<Street> allStreets = new ArrayList<Street>();
    public List<BusLine> allBusLines = new ArrayList<BusLine>();
    public List<Vehicle> allVehicles = new ArrayList<Vehicle>();

    public String selectedBusId = "";

    @FXML
    private Pane content;
    @FXML
    private TextField timeScale;
    @FXML
    private Slider sliditko;
    @FXML
    private Label timeLabel;

    private List<Drawable> elements = new ArrayList<>();
    private List<TimeUpdate> updates = new ArrayList<>();
    private Timer timer;
    private LocalTime time = LocalTime.parse("08:00:00");
    private LocalTime delayTime = time;
    private LocalTime delayTime1 = time;
    private boolean starter = false;


    /**
     * Funkce pro priblizovani a oddalovani mapoveho podkladu
     * @param event ScrollEvent pohyb kolecka mysi
     */
    @FXML
    private void onZoom(ScrollEvent event){
        event.consume();

        double zoom = event.getDeltaY() >= 0 ? 1.05 : 0.9;
        content.setScaleX(zoom * content.getScaleX());
        content.setScaleY(zoom * content.getScaleY());
        content.layout();
    }

    /**
     * Funkce je volana pohybem slideru
     * Nastavi rychlost casu podle slideru
     */
    @FXML
    private void onTimeScaleChange() {
        try {

            double hodnota = sliditko.getValue();
            timeScale.setText(Integer.toString((int)hodnota));
            timer.cancel();
            startTime((float)hodnota);

        }
        catch (NumberFormatException e) {
            Alert alert = new Alert(Alert.AlertType.ERROR, "Spatny format casoveho broniku");
            alert.showAndWait();
            return;
        }
    }


    /**
     * Pridava graficke objekty do objektu Pane, vykresluje mapu, zastavky a vozidla
     * @param elements List Drawable  seznam grafickych objektu
     */
    public void setElements(List<Drawable> elements) {
        this.elements = elements;
        for (Drawable drawable : elements) {
            content.getChildren().addAll(drawable.getGUI());
            if (drawable instanceof TimeUpdate) {
                updates.add((TimeUpdate)drawable);
            }
        }
    }

    /**
     * Nastavuje rychlost timeru podle hodnoty scale
     * @param scale float rychlost casu
     */
    public void startTime(float scale) {
        if(scale < 0) scale = 1;
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
            Platform.runLater(() -> { time = time.plusSeconds(1);
                for (TimeUpdate update : updates) {
                    update.update(time);
                }
                timeLabel.setText(time.toString());
                actualizeBusTrack(selectedBusId);
                resetBuses();
            });
            }
        }, 10, (long) (1000 / scale));
    }

    /**
     * Pomocna funkce pro nahrani souboru do Stringu
     * @param filePath String cesta k souboru
     * @return String obsah souboru
     */
    private static String readAllBytesJava7(String filePath)
    {
        String content = "";
        try{
            content = new String ( Files.readAllBytes( Paths.get(filePath) ) );
        }
        catch (IOException e){
            e.printStackTrace();
        }
        return content;
    }

    /**
     * Funkce volana stiskem tlacitka pro otevreni mapy
     * Parsuje soubor s ulicemi a zastavkami ve tvaru JSON. Jednotlive ulice pridava do listu allStreets a zastavky do allStops.
     * Pridava zastavky k prislusnym ulicim.
     */
    @FXML
    private void mapBtnAction(){
        FileChooser fc = new FileChooser();
        File selectedFile = fc.showOpenDialog(null);

        content.getChildren().clear();
        elements.clear();
        allStreets.clear();
        allStops.clear();
        starter = false;

        if(selectedFile != null){
            String filecontent;
            String filepath = selectedFile.toString();
            filecontent = readAllBytesJava7(filepath);

            try {
                JSONObject json_obj = new JSONObject(filecontent);
                JSONArray json_streets = json_obj.getJSONArray("Streets");
                JSONArray json_stop = json_obj.getJSONArray("Stops");
            }
            catch (Exception e){
                Alert alert = new Alert(Alert.AlertType.ERROR, "Spatny format json pro mapy");
                alert.showAndWait();
                return;
            }

            JSONObject json_obj = new JSONObject(filecontent);

            JSONArray json_streets = json_obj.getJSONArray("Streets");
            JSONArray json_stop = json_obj.getJSONArray("Stops");

            for (int i = 0; i < json_streets.length(); i++) {
                String street_id = json_streets.getJSONObject(i).getString("id");
                JSONArray json_coords = json_streets.getJSONObject(i).getJSONArray("coords");

                List<Coordinate> coordinates = new ArrayList<Coordinate>();
                for(int j = 0; j < json_coords.length(); j++){
                    JSONArray json_xy = json_coords.getJSONArray(j);
                    double x = json_xy.optInt(0);
                    double y = json_xy.optInt(1);

                    Coordinate c = new Coordinate(x, y);
                    coordinates.add(c);
                }
                Street street = new Street(street_id, coordinates);
                allStreets.add(street);
            }

            for (int i = 0; i < json_stop.length(); i++) {
                String stop_id = json_stop.getJSONObject(i).getString("id");
                JSONArray json_coords = json_stop.getJSONObject(i).getJSONArray("coords");

                int x = json_coords.optInt(0);
                int y = json_coords.optInt(1);

                Coordinate c = new Coordinate(x, y);

                Stop stop = new Stop(stop_id, c);
                allStops.add(stop);
            }

            for (Street street : allStreets){
                for(Stop stop : allStops){
                    street.addStop(stop);
                }
            }
            for(Street s: this.allStreets){
                elements.add(s);
            }
        }
    }

    /**
     * Funkce volana stiskem tlacitka pro otevreni linek.
     * Parsuje soubor s linkama ve formatu JSON. Vytvari autobusove linky s prislusnymi ulicemi a zastavkami.
     * Pro kazdou linku vytvori jedno vozidlo.
     */
    @FXML
    private void lineBtnAction() {
        FileChooser fc = new FileChooser();
        File selectedFile = fc.showOpenDialog(null);

        allBusLines.clear();

        if (selectedFile != null) {
            String filecontent;
            String filepath = selectedFile.toString();
            filecontent = readAllBytesJava7(filepath);

            try {
                JSONObject json_obj = new JSONObject(filecontent);
                JSONArray json_busLines = json_obj.getJSONArray("Lines");
            }
            catch (Exception e){
                Alert alert = new Alert(Alert.AlertType.ERROR, "Spatny format json pro linky");
                alert.showAndWait();
                return;
            }
            if(allStreets.isEmpty()){
                Alert alert = new Alert(Alert.AlertType.ERROR, "Nejsou nahrany mapy");
                alert.showAndWait();
                return;
            }

            JSONObject json_obj = new JSONObject(filecontent);
            JSONArray json_busLines = json_obj.getJSONArray("Lines");


            for (int i = 0; i < json_busLines.length(); i++) {
                String busline_id = json_busLines.getJSONObject(i).getString("id");
                JSONArray json_route = json_busLines.getJSONObject(i).getJSONArray("route");

                String streetname = "";
                String stopname = "";
                int houros = 99;
                int minutos = 99;

                BusLine tempBL = new BusLine(busline_id);

                for (int j = 0; j < json_route.length(); j++) {
                    JSONArray json_info = json_route.getJSONArray(j);
                    streetname = json_info.getString(0);
                    stopname = json_info.getString(1);
                    houros = json_info.getInt(2);
                    minutos = json_info.getInt(3);

                    for (Street stretos : allStreets) {
                        if (streetname.compareTo(stretos.getId()) == 0) {
                            if (stopname.compareTo("0") != 0) {
                                for (Stop stopos : stretos.getStops()) {
                                    if (stopname.compareTo(stopos.getId()) == 0) {
                                        tempBL.addStreet(stretos, stopos);
                                    }
                                }
                            } else tempBL.addStreet(stretos);
                        }
                    }
                }
                allBusLines.add(tempBL);
            }
        }
        for(BusLine b : allBusLines){
            Vehicle tempVeh = new Vehicle(1, b);
            this.elements.add(tempVeh);
            allVehicles.add(tempVeh);
        }
    }

    /**
     * Funkce po stisku tlacitka spusti simulaci autobusove dopravy
     */
    @FXML
    private void runSimAction(){
        if(starter) return;
        this.setElements(elements);
        elements.clear();
        time = LocalTime.parse("08:00:00");
        starter = true;
    }

    @FXML
    private TextArea busLineTracker;

    /**
     * Funkce se aktivuje stisknutim leveho mysitka. Pri dvojkliku na ulici vybranou ulici zpomali.
     * Pri kliknuti na autobus zobrazi jeho trasu a jizdni rad.
     * @param mouseEvent MouseEvent stisk tlacitka mysi
     */
    @FXML
    private void selectBusLine(MouseEvent mouseEvent){
        double mouseEnterX = mouseEvent.getX();
        double mouseEnterY = mouseEvent.getY();
        Coordinate mouse = Coordinate.create(mouseEnterX, mouseEnterY);

        selectedBusId = "";

        if(mouseEvent.getClickCount() == 2){
            for (Street s : allStreets) {
                if (s.inLine(mouse)) {
                    if (s.isSlow) {
                        s.isSlow = false;
                    } else {
                        s.isSlow = true;
                    }
                }
            }
        }

        for(Street s: allStreets){
            s.setColor(Color.BLACK);
            if(s.isSlow) s.setColor(Color.RED);
        }
        for(Vehicle veh : allVehicles){
            veh.setColor(Color.BLUE);
            if((mouse.diffX(veh.position) <= 4 && mouse.diffX(veh.position) >= -4) &&
                (mouse.diffY(veh.position) <= 4 && mouse.diffY(veh.position) >= -4)){
                veh.setColor(Color.GREEN);
                selectedBusId = veh.buslineId;

                for(BusLine bl : allBusLines){
                    if(bl.getId().compareTo(veh.buslineId) == 0){
                        for(Street s: bl.getStreets()){
                            s.setColor(Color.GREEN);
                            if(s.isSlow) s.setColor(Color.ORANGE);
                        }
                    }
                }
            }
        }
    }

    /**
     * Resetuje autobusy kazdych 30 simulacnich minut. Autobusy s kratsi trasou kazdych 15 minut.
     */
    private void resetBuses(){
        if(time.compareTo(delayTime1.plusSeconds(900)) == 0){
            for(Vehicle veh : allVehicles){
                if(veh.pathSize < 500){
                    veh.reset();
                    delayTime1 = time;
                }
            }
        }

        if(time.compareTo(delayTime.plusSeconds(1800)) == 0){
            for(Vehicle veh : allVehicles){
                if(veh.pathSize >= 500){
                    veh.reset();
                    delayTime = time;
                }
            }
        }
    }


    private Stop tempStop = new Stop("temp",Coordinate.create(0,0));

    /**
     * Stara se o znovu rozjeti autobusu ze zastavky po 30ti sekundach, ridi rychlost autobusu na zpomalenych silnicich.
     * Vypisuje jizdni rady a aktualni zastavku zvoleneho autobusu do textoveho pole.
     * @param selected String vybrany autobus
     */
    private void actualizeBusTrack(String selected){
        if(allVehicles.isEmpty()) return;
        for(Vehicle veh : allVehicles){
            if(veh.speed == 0){
                if(time.compareTo(veh.stopTime.plusSeconds(30)) == 0){
                    veh.speed = 1;
                }
            }
            for (Street s : allStreets){
                if(s.inLine(veh.position)){
                    if(veh.speed == 0) veh.speed = 0;
                    else if(s.isSlow){
                        veh.speed = 0.5;
                    }
                    else if(veh.speed == 0.5) veh.speed = 1;
                }
            }
        }

        if(selected.compareTo("") == 0){
            busLineTracker.setText("");
            return;
        }
        for(Vehicle veh : allVehicles){
            if(veh.buslineId.compareTo(selected) == 0){
                if(!tempStop.equals(veh.lastStop)){

                    busLineTracker.setText("");

                    for(BusLine bl : allBusLines) {
                        if(bl.getId().compareTo(selected) == 0){
                            busLineTracker.appendText(bl.getId() + "\n\n");
                            for (Stop stop : bl.getStops()) {
                                busLineTracker.appendText(stop.getId());
                                if (stop.equals(veh.lastStop)) {
                                    busLineTracker.appendText(" - predchozi");
                                }
                                busLineTracker.appendText("\n");
                            }
                        }
                    }
                    tempStop = veh.lastStop;
                }
            }
        }
    }
}
