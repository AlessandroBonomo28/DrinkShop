package com.uninaproject.juicylemon.events;

import com.uninaproject.juicylemon.model.Drink;

import java.util.List;

public class DrinkFetchedEvent {
    List<Drink> drinks;

    public DrinkFetchedEvent(List<Drink> drinks) {
        this.drinks = drinks;
    }

    public List<Drink> getDrinks() {
        return drinks;
    }
}
