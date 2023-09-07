package com.uninaproject.juicylemon.view_models;

import android.content.Context;
import android.util.Log;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.events.FetchedLastOrderFromServer;
import com.uninaproject.juicylemon.model.Drink;

import org.greenrobot.eventbus.EventBus;

import java.util.Map;

public class CartViewModel extends ViewModel {

    private MutableLiveData<Map<Drink, Integer>> drinks;

    public CartViewModel() {
        Controller controller = Controller.getInstance();
        drinks = new MutableLiveData<>(controller.getCart().getDrinksMap());
    }

    public MutableLiveData<Map<Drink, Integer>> getDrinks() {
        return drinks;
    }

    public void addDrink(Drink drink) {
        Controller.getInstance().getCart().addDrink(drink);
        drinks.postValue(Controller.getInstance().getCart().getDrinksMap());
    }

    public void sendOrder(Context context) {
        Controller.getInstance().getOrderDAO().pushCurrentCartToServer(Controller.getInstance().getCart(), context);
    }

    public void clearCart() {
        Controller.getInstance().getCart().clear();
        drinks.postValue(Controller.getInstance().getCart().getDrinksMap());
    }

    public void removeDrink(Drink drink) {
        Controller.getInstance().getCart().removeAmountOfDrink(drink, 1);
        drinks.postValue(Controller.getInstance().getCart().getDrinksMap());
    }
}
