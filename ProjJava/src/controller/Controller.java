package controller;

import model.GestVendas;
import view.*;

import java.io.IOException;

public class Controller implements IController{
    private View view;
    private GestVendas gestVendas;


    public void runController() throws IOException {
        /* Grab the option with the menu form */
        int option = view.firstMenuForm();
        while(option != 0) {
            if(!model.isInitialized() && option <= 12){
                view.printNotLoadedMessage();
            }
            else switch (option) {
                case 1:
                    query1_allNotBoughtProducts();
                    break;
                case 2:
                    try {
                        query2_globalSalesOnMonthInBranch();
                    } catch (InvalidMonthException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 3:
                    try {
                        query3_infoAboutCustomer();
                    } catch (InvalidCustomerException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 4:
                    try {
                        query4_productTotalMadeAndTotalCustomers();
                    } catch (InvalidProductException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 5:
                    try {
                        query5_productsBoughtByCustomer();
                    } catch (InvalidCustomerException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 6:
                    try {
                        query6_XProductsMostSold();
                    } catch (OutOfBoundsException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 7:
                    query7_3BiggestSpendersPerBranch();
                    break;
                case 8:
                    try {
                        query8_BiggestBuyersOfDifferentProducts();
                    } catch (OutOfBoundsException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 9:
                    try {
                        query9_biggestBuyersOfCertainProduct();
                    } catch (InvalidProductException | OutOfBoundsException e) {
                        view.displayError(e.getMessage());
                    }
                    break;
                case 10:
                    query10_productsBilling();
                    break;
                case 11:
                    query11_lastFileInfo(); break;
                case 12:
                    query12_currentModelInfo(); break;
                case 13:
                    loadFilesThroughPaths(); break;
                case 14:
                    loadDefaultFiles(); break;
                default:
                    view.menuErrorMessage();
                    break;
            }
            view.clearScreen();
            option = view.menuForm();
        }
    }
}
}
