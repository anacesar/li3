package exceptions;

public class FilialInvalida extends Exception{
    public FilialInvalida(){
        super("A filial introduzida não é válida.");
    }
    public FilialInvalida(String message) {
        super(message);
    }
}
