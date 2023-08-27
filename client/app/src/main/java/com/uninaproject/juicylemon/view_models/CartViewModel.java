package com.uninaproject.juicylemon.view_models;

import static com.uninaproject.juicylemon.utils.Utils.API_BASE_URL;

import android.content.Context;
import android.util.Log;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.android.volley.Request;
import com.google.android.material.snackbar.Snackbar;
import com.uninaproject.juicylemon.Controller;
import com.uninaproject.juicylemon.activities.DashboardActivity;
import com.uninaproject.juicylemon.events.CartPushedEvent;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.utils.LoginManager;
import com.uninaproject.juicylemon.utils.RequestSender;

import org.greenrobot.eventbus.EventBus;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
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

    public void addDrink(Drink drink) {
        Controller.getInstance().getCart().addDrink(drink);
        drinks.postValue(Controller.getInstance().getCart().getDrinks());
    }

    public void sendOrder(Context context) {
        Log.d("CartViewModel", "sendOrder: " + Controller.getInstance().getCart().getDrinks().toString());
        Controller.getInstance().getOrderDAO().pushCurrentCartToServer(Controller.getInstance().getCart(), context);


    }

    public void clearCart() {
        Controller.getInstance().getCart().clear();
        drinks.postValue(Controller.getInstance().getCart().getDrinks());
    }

    public void removeDrink(Drink drink) {
        Controller.getInstance().getCart().removeAmountOfDrink(drink, 1);
        drinks.postValue(Controller.getInstance().getCart().getDrinks());
    }
}
