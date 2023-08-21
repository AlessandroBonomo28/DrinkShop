package com.uninaproject.juicylemon.view_models;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.uninaproject.juicylemon.Controller;

public class NavbarViewModel extends ViewModel {

    private final MutableLiveData<Integer> cartSize;

    public NavbarViewModel() {
        int cartGlobalSize = Controller.getInstance().getCart().getTotalSize();

        cartSize = new MutableLiveData<>(cartGlobalSize);
    }

    public MutableLiveData<Integer> getCartSize() {
        return cartSize;
    }

    public void setCartSize(int size) {
        cartSize.setValue(size);
    }

}
