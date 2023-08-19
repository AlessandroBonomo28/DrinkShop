package com.uninaproject.juicylemon.events;

public class UserAuthErrorEvent {

    private String message;

    private final String DEFAULT_MESSAGE = "Si è verificato un errore";

    public UserAuthErrorEvent(String message) {
        if (message == null || message.isEmpty())
            this.message = DEFAULT_MESSAGE;
        else
            this.message = message;
    }

    public String getMessage() {
        return message;
    }
}
