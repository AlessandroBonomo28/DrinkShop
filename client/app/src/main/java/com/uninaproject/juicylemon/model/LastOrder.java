package com.uninaproject.juicylemon.model;

import java.util.List;

public class LastOrder {

    private List<Drink> drinks;

    public LastOrder(List<Drink> drinks) {
        this.drinks = drinks;
    }

    public List<Drink> getDrinks() {
        return drinks;
    }
}
