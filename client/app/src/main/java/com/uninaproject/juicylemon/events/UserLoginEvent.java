package com.uninaproject.juicylemon.events;

import com.uninaproject.juicylemon.daos.TokenPayload;


public class UserLoginEvent {

    public TokenPayload tokenPayload;

    public UserLoginEvent(TokenPayload tokenPayload) {
        this.tokenPayload = tokenPayload;
    }

}
