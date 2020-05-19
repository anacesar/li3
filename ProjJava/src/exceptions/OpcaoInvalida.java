package exceptions;

public class OpcaoInvalida extends Exception {
    public OpcaoInvalida(){
        super("A opção inserida não é válida.");
    }
    public OpcaoInvalida(String message) {
        super(message);
    }
}
