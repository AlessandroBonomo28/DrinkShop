package com.uninaproject.juicylemon.view_models;

import androidx.lifecycle.ViewModel;

import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.events.CartUpdateEvent;
import com.uninaproject.juicylemon.model.Drink;

import org.greenrobot.eventbus.EventBus;

public class DetailActivityViewModel extends ViewModel {
    
    public void addDrinkToCart(Drink drink) {
        Controller.getInstance().addDrinkToCart(drink);
    }

}
