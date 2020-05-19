package exceptions;

public class ProdutoInvalido extends Exception {
    public ProdutoInvalido(){
        super("O código de produto inserido não é válido.");
    }
    public ProdutoInvalido(String message) {
        super(message);
    }
}
