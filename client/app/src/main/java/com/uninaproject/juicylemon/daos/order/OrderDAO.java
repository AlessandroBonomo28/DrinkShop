package com.uninaproject.juicylemon.daos.order;

import android.content.Context;

import com.uninaproject.juicylemon.model.Cart;

public interface OrderDAO {

    public void pushCurrentCartToServer(Cart cart, Context context);

    public void fetchOrdersFromServer(Context context);

}
