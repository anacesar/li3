package exceptions;

public class MesInvalido extends Exception {
    public MesInvalido() {
        super("O mês inserido não é válido.");
    }

    public MesInvalido(String message) {
        super(message);
    }
}
