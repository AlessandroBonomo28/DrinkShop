package com.uninaproject.juicylemon.utils;

import com.uninaproject.juicylemon.daos.TokenPayload;
import com.uninaproject.juicylemon.model.Drink;
import com.uninaproject.juicylemon.model.User;

import java.util.ArrayList;
import java.util.List;

public class LoginManager {

    private static LoginManager _instance;

    User user;
    TokenPayload tokenPayload;

    private LoginManager() {
        user = null;
        tokenPayload = null;

    }

    public static LoginManager getInstance() {
        if (_instance == null) {
            _instance = new LoginManager();
        }
        return _instance;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void setTokenPayload(TokenPayload tokenPayload) {
        this.tokenPayload = tokenPayload;
        this.user = Utils.extractUserFromTokenPayload(tokenPayload);
    }

    public User getUser() {
        return user;
    }
    
    public TokenPayload getTokenPayload() {
        return tokenPayload;
    }
}
