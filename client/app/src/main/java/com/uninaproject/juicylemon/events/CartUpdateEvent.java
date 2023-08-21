package com.uninaproject.juicylemon.events;

public class CartUpdateEvent {

    private int cartSize;

    public CartUpdateEvent(int cartSize) {
        this.cartSize = cartSize;
    }

    public int getCartSize() {
        return cartSize;
    }
}
