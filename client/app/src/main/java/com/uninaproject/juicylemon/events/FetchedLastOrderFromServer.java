package com.uninaproject.juicylemon.events;

import com.uninaproject.juicylemon.model.Drink;

import java.util.List;

public class FetchedLastOrderFromServer {

    List<Drink> drinksFetched;

    public FetchedLastOrderFromServer(List<Drink> drinksFetched) {
        this.drinksFetched = drinksFetched;
    }

    public List<Drink> getDrinksFetched() {
        return drinksFetched;
    }
}
