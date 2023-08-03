package com.uninaproject.juicylemon;

import android.content.Context;
import android.content.DialogInterface;
import android.widget.EditText;

import androidx.appcompat.app.AlertDialog;

import java.util.Collection;
import java.util.List;
import java.util.Optional;

public class Utils {

    public static long getAllFieldsNotEmpty(Collection<? extends EditText> fields) {
        long emptyFields = fields.stream().filter(field -> field.getText().toString().isEmpty()).count();
        return emptyFields;

    }

    public static void showAlert(Context context, String message) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Attenzione");
        builder.setMessage(message);
        builder.setPositiveButton("OK", null);
        builder.show();
    }
}
