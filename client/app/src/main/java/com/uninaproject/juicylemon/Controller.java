package com.uninaproject.juicylemon;

import com.uninaproject.juicylemon.daos.UserDAO;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAO;
import com.uninaproject.juicylemon.daos.drinks.DrinkDAOImpl;
import com.uninaproject.juicylemon.daos.order.OrderDAO;
import com.uninaproject.juicylemon.daos.order.OrderDAOImpl;
import com.uninaproject.juicylemon.events.CartUpdateEvent;
import com.uninaproject.juicylemon.model.Cart;
import com.uninaproject.juicylemon.model.Drink;

import org.greenrobot.eventbus.EventBus;

public class Controller {

    private static Controller instance;

    DrinkDAO drinkDAO;
    UserDAO userDAO;

    OrderDAO orderDAO;

    Cart cart;

    private Controller() {
        drinkDAO = new DrinkDAOImpl();
        orderDAO = new OrderDAOImpl();
        userDAO = new UserDAO();
        cart = new Cart();
    }

    public static Controller getInstance() {
        if (instance == null)
            instance = new Controller();
        return instance;
    }

    public DrinkDAO getDrinkDAO() {
        return drinkDAO;
    }

    public UserDAO getUserDAO() {
        return userDAO;
    }

    public OrderDAO getOrderDAO() {
        return orderDAO;
    }

    public Cart getCart() {
        return cart;
    }

    public void addDrinkToCart(Drink drink) {
        this.getCart().addDrink(drink);
        EventBus.getDefault().post(new CartUpdateEvent(this.getCart().getTotalSize()));
    }
}
