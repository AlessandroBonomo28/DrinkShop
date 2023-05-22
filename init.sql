CREATE TABLE "Users" (
  "id" SERIAL PRIMARY KEY,
  "password" VARCHAR(200) NOT NULL,
  "email" VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE "Drinks" (
  "id" SERIAL PRIMARY KEY,
  "name" VARCHAR(50),
  "description" VARCHAR(250),
  "price" float CHECK ("price" > 0)
);

CREATE TABLE "Payments" (
  "id" SERIAL PRIMARY KEY,
  "id_order" int NOT NULL UNIQUE,
  "id_user" int NOT NULL,
  "card_name" VARCHAR(50),
  "creation_timestamp" timestamp DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE "Orders" (
  "id" SERIAL PRIMARY KEY,
  "id_user" int NOT NULL,
  "creation_timestamp" timestamp DEFAULT CURRENT_TIMESTAMP,
  "paid" boolean DEFAULT false
);

CREATE TABLE "OrderItems" (
  "id" SERIAL PRIMARY KEY,
  "id_order" int NOT NULL,
  "id_item" int NOT NULL,
  "quantity" float CHECK ("quantity" > 0)
);

ALTER TABLE "OrderItems" ADD FOREIGN KEY ("id_order") REFERENCES "Orders" ("id");
ALTER TABLE "OrderItems" ADD FOREIGN KEY ("id_item") REFERENCES "Drinks" ("id");
ALTER TABLE "Orders" ADD FOREIGN KEY ("id_user") REFERENCES "Users" ("id");
ALTER TABLE "Payments" ADD FOREIGN KEY ("id_order") REFERENCES "Orders" ("id");
ALTER TABLE "Payments" ADD FOREIGN KEY ("id_user") REFERENCES "Users" ("id");

-- Creazione della funzione PL/pgSQL per verificare che l'utente che effettua il pagamento sia lo stesso dell'ordine
CREATE OR REPLACE FUNCTION check_payment_user_match_order()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM "Orders"
        WHERE "id" = NEW."id_order"
        AND "id_user" = NEW."id_user"
    ) THEN
        RAISE EXCEPTION 'L''utente che effettua il pagamento non corrisponde all''utente dell''ordine.';
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Creazione del trigger per eseguire la verifica durante l'inserimento di un pagamento
CREATE TRIGGER check_payment_user_match_order_trigger
BEFORE INSERT ON "Payments"
FOR EACH ROW
EXECUTE FUNCTION check_payment_user_match_order();

-- procedura per aggiornare lo stato di pagamento
CREATE OR REPLACE FUNCTION update_paid_status()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE "Orders"
    SET "paid" = TRUE
    WHERE "id" = NEW."id_order";
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Creazione del trigger per il pagamento
CREATE TRIGGER update_order_paid_status
AFTER INSERT ON "Payments"
FOR EACH ROW
EXECUTE FUNCTION update_paid_status();

-- Creazione della funzione di trigger per verificare lo stato di pagamento dell'ultimo ordine dell'utente
CREATE OR REPLACE FUNCTION check_last_order_payment_status()
RETURNS TRIGGER AS $$
DECLARE
    last_order_paid BOOLEAN;
BEGIN
    -- Verifica lo stato di pagamento dell'ultimo ordine dell'utente
    SELECT "paid" INTO last_order_paid
    FROM "Orders"
    WHERE "id_user" = NEW."id_user"
    ORDER BY "timestamp" DESC
    LIMIT 1;
    
    -- Se l'ultimo ordine non è stato pagato, lancia un'eccezione per impedire l'inserimento del nuovo ordine
    IF last_order_paid = FALSE THEN
        RAISE EXCEPTION 'Non è possibile creare un nuovo ordine se l''ultimo ordine risulta non pagato.';
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Creazione del trigger per verificare lo stato di pagamento dell'ultimo ordine dell'utente
CREATE TRIGGER check_last_order_payment_status_trigger
BEFORE INSERT ON "Orders"
FOR EACH ROW
EXECUTE FUNCTION check_last_order_payment_status();

INSERT INTO "Users" ("email","password") VALUES
    ('alex@gmail.com','123'),
    ('mario@gmail.com','123');

INSERT INTO "Drinks" ("name", "description", "price") VALUES
    ('Bevanda 1', 'Descrizione bevanda 1', 2.5),
    ('Bevanda 2', 'Descrizione bevanda 2', 3.0),
    ('Bevanda 3', 'Descrizione bevanda 3', 2.0),
    ('Bevanda 4', 'Descrizione bevanda 4', 2.75),
    ('Bevanda 5', 'Descrizione bevanda 5', 3.5);

