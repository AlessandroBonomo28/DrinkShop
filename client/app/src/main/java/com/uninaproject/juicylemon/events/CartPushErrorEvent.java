package com.uninaproject.juicylemon.events;

public class CartPushErrorEvent {

    private String message;

    public CartPushErrorEvent(String message) {
        this.message = message;
    }

    public String getMessage() {
        return message;
    }
}
