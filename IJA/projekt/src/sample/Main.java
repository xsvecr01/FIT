/**
 * Hlavni trida - vytvari okno a scenu
 * @author Michal Dostal (xdosta51)
 * @author Radek Svec (xsvecr01)
 * 
 */
package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;


public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        FXMLLoader loader =  new FXMLLoader(getClass().getResource("sample.fxml"));
        BorderPane root = loader.load();
        Scene scene = new Scene(root, 800 ,600);
        primaryStage.setMinHeight(600);
        primaryStage.setMinWidth(800);
        primaryStage.setScene(scene);
        primaryStage.setTitle("IJA - xdosta51-xsvecr01");

        primaryStage.show();

        primaryStage.setOnCloseRequest(event -> System.exit(0));
        Controller controller = loader.getController();
        //List<Drawable> elements = new ArrayList<>();
        controller.startTime(1);
    }


    public static void main(String[] args) {
        launch(args);
    }
}
