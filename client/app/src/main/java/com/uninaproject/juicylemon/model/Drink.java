package com.uninaproject.juicylemon.model;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Drink implements Serializable {
    public int id;
    public String name;
    public float price;
    public String description;
    public DrinkType type;

    public Drink(int id, String name, float price, String description, DrinkType type) {
        this.id = id;
        this.name = name;
        this.price = price;
        this.description = description;
        this.type = type;
    }

    public Drink(String name, DrinkType type, float price) {
        this.name = name;
        this.type = type;
        this.price = price;
    }

    public static Drink fromJSON(JSONObject object) {
        return fromJSON(object, "id");
    }

    public static Drink fromJSON(JSONObject object, String overrideIdName) {
        try {

            DrinkType drinkType = object.getString("drink_type").equals("cocktail") ? DrinkType.COCKTAIL : DrinkType.FRULLATO;

            return new Drink(object.getInt(overrideIdName), object.getString("name"), Float.parseFloat(object.getString("price")), object.getString("description"), drinkType);
        } catch (JSONException e) {
            throw new RuntimeException(e);
        }
    }

    public static List<Drink> fromJsonArray(JSONArray array) {
        return Stream.iterate(0, i -> i + 1)
                .limit(array.length())
                .map(i -> {
                    try {
                        return array.getJSONObject(i);
                    } catch (JSONException e) {
                        throw new RuntimeException(e);
                    }
                })
                .map(Drink::fromJSON)
                .collect(Collectors.toList());
    }

    public String getName() {
        return name;
    }

    public float getPrice() {
        return price;
    }

    public String getDescription() {
        return description;
    }

    public DrinkType getType() {
        return type;
    }


    @Override
    public String toString() {
        return "Drink{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", price=" + price +
                ", description='" + description + '\'' +
                ", type=" + type +
                '}';
    }

    public int getId() {
        return id;
    }
}
