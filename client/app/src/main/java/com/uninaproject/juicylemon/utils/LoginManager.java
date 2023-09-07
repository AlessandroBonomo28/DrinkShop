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

    private List<Drink> localLastOrder;

    private LoginManager() {
        user = null;
        tokenPayload = null;
        localLastOrder = new ArrayList<>();

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

    public void setLocalLastOrder(List<Drink> localLastOrder) {
        this.localLastOrder = localLastOrder;
    }

    public List<Drink> getLocalLastOrder() {
        return localLastOrder;
    }

    public TokenPayload getTokenPayload() {
        return tokenPayload;
    }
}
