package com.uninaproject.juicylemon.view_models;

import android.content.Context;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.uninaproject.juicylemon.model.Drink;

import java.util.List;

public class DrinksListViewModel extends ViewModel {

    public MutableLiveData<List<Drink>> drinks;
    public  MutableLiveData<String> searchQuery;

    private List<Drink> defaultDrinks;

    public LiveData<String> getSearchQuery() {
        return searchQuery;
    }

    public void setSearchQuery(String query) {
        searchQuery.setValue(query);
    }

    public DrinksListViewModel() {
        searchQuery = new MutableLiveData<>();
        drinks = new MutableLiveData<>();
    }

    public MutableLiveData<List<Drink>> getDrinks() {
        return drinks;
    }

    public void setDefaultDrinks(List<Drink> defaultDrinks) {
        this.defaultDrinks = defaultDrinks;
    }

    public List<Drink> getDefaultDrinks() {
        return defaultDrinks;
    }
}

