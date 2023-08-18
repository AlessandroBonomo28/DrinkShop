package com.uninaproject.juicylemon.utils;

import android.content.Context;
import android.content.DialogInterface;
import android.util.Pair;
import android.widget.EditText;

import androidx.appcompat.app.AlertDialog;

import com.uninaproject.juicylemon.daos.TokenPayload;
import com.uninaproject.juicylemon.model.User;

import java.util.Collection;
import java.util.List;
import java.util.Optional;

public class Utils {

    public final static String API_BASE_URL = "http://192.168.144.86:80/api/";

    /**
     * Check if all the fields are not empty
     * @param fields the fields to check
     * @return true if all the fields are not empty, false otherwise
     */
    public static long getAllFieldsNotEmpty(Collection<? extends EditText> fields) {
        long emptyFields = fields.stream().filter(field -> field.getText().toString().isEmpty()).count();
        return emptyFields;

    }

    /**
     * Show an alert with a custom message
     * @param context the context
     * @param message the message to show
     */
    public static void showAlert(Context context, String message) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Attenzione");
        builder.setMessage(message);
        builder.setPositiveButton("OK", null);
        builder.show();
    }

    /**
     * Show an alert with a custom title and message
     * @param context the context
     * @param title the title to be shown
     * @param message the message to show
     */
    public static void showAlert(Context context, String title, String message) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(title);
        builder.setMessage(message);
        builder.setPositiveButton("OK", null);
        builder.show();
    }

    public static User extractUserFromTokenPayload(TokenPayload tokenPayload) {
        return new User(tokenPayload.id, tokenPayload.email);
    }

    public static Pair<String, String> separateEmail(String email) {
        String[] splitted = email.split("@");
        return new Pair<>(splitted[0], splitted[1]);
    }

    public static String capitalize(String str) {
        return str.substring(0, 1).toUpperCase() + str.substring(1);
    }

    public static String addCurrencySymbol(String str) {
        return str + "€";
    }
}
