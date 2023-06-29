package com.uninaproject.juicylemon.model;

public class Drink {
    public int id;
    public String name;
    public float price;
    public String description;
    public DrinkType type;

    public Drink(int id, String name, float price, String description, DrinkType type) {
        this.id = id;
        this.name = name;
        this.price = price;
        this.description = description;
        this.type = type;
    }
}
