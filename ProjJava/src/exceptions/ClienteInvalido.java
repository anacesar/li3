package exceptions;

public class ClienteInvalido extends Exception{
    public ClienteInvalido(){
        super("O código de cliente inserido não é válido.");
    }
    public ClienteInvalido(String message) {
        super(message);
    }
}
