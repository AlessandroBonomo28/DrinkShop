package com.uninaproject.juicylemon.view_models;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.model.Drink;

import java.util.Map;

public class CartViewModel extends ViewModel {

    private MutableLiveData<Map<Drink, Integer>> drinks;

    public CartViewModel() {
        Controller controller = Controller.getInstance();

        drinks = new MutableLiveData<>(controller.getCart().getDrinks());
    }

    public MutableLiveData<Map<Drink, Integer>> getDrinks() {
        return drinks;
    }
}
