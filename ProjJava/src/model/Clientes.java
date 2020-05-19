package model;

import model.Interfaces.IClientes;

import java.io.Serializable;
import java.util.Map;

public class Clientes implements IClientes, Serializable {
    private Map<Produto, String> produtos;
}
