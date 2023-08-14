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

    public Drink(String name, DrinkType type, float price) {
        this.name = name;
        this.type = type;
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public float getPrice() {
        return price;
    }

    public String getDescription() {
        return description;
    }

    public DrinkType getType() {
        return type;
    }
}
