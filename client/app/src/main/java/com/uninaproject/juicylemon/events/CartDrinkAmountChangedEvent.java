package com.uninaproject.juicylemon.events;

import com.uninaproject.juicylemon.model.Drink;

public class CartDrinkAmountChangedEvent {

    public enum  Action {
        ADD,
        REMOVE
    }

    private final Drink drink;
    private final int newAmount;

    private final Action action;

    public CartDrinkAmountChangedEvent(Drink drink, int newAmount, Action action) {
        this.drink = drink;
        this.newAmount = newAmount;
        this.action = action;
    }

    public Drink getDrink() {
        return drink;
    }

    public int getNewAmount() {
        return newAmount;
    }

    public Action getAction() {
        return action;
    }
}
