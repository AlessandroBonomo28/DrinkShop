package com.uninaproject.juicylemon.model;

import com.uninaproject.juicylemon.events.CartUpdateEvent;

import org.greenrobot.eventbus.EventBus;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Cart {

    Map<Drink, Integer> drinkInCarrello;
    float totale;

    public Cart() {
        totale = 0;
        drinkInCarrello = new HashMap<>();
    }

    /**
     *
     * @return the amount of unique drinks in the cart
     */
    public int getSize() {
        return drinkInCarrello.size();
    }


    /**
     *
     * @return the total amount of drinks in the cart (sum of all the drinks)
     */
    public int getTotalSize() {
        return drinkInCarrello.values().stream().mapToInt(Integer::intValue).sum();
    }

    public void addDrink(Drink drink) {
        if (drinkInCarrello.containsKey(drink))
            drinkInCarrello.put(drink, drinkInCarrello.get(drink) + 1);
        else
            drinkInCarrello.put(drink, 1);

        EventBus.getDefault().post(new CartUpdateEvent(getTotalSize()));
    }

    public void removeAmountOfDrink(Drink drink, int amount) {
        if (amount <= 0)
            throw new IllegalArgumentException("amount must be positive");

        if (drinkInCarrello.containsKey(drink)) {
            int newAmount = drinkInCarrello.get(drink) - amount;
            if (newAmount <= 0)
                drinkInCarrello.remove(drink);
            else
                drinkInCarrello.put(drink, newAmount);
        }

        EventBus.getDefault().post(new CartUpdateEvent(getTotalSize()));
    }

    public Map<Drink, Integer> getDrinksMap() {
        return drinkInCarrello;
    }

    public List<Drink> getDrinksList() {
        return (List<Drink>) drinkInCarrello.keySet();
    }

    public void clear() {
        drinkInCarrello.clear();
    }
}
