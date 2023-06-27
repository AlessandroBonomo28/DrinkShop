CREATE TYPE DRINK_TYPE AS ENUM ('cocktail', 'frullato');

CREATE TABLE "Users" (
  "id" SERIAL PRIMARY KEY,
  "password" VARCHAR(200) NOT NULL,
  "email" VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE "Drinks" (
  "id" SERIAL PRIMARY KEY,
  "name" VARCHAR(50),
  "description" VARCHAR(250),
  "image_url" VARCHAR(250),
  "price" float CHECK ("price" > 0),
  "drink_type" DRINK_TYPE DEFAULT 'cocktail'
);

CREATE TABLE "Payments" (
  "id" SERIAL PRIMARY KEY,
  "id_order" int NOT NULL UNIQUE,
  "id_user" int NOT NULL,
  "card_holder" VARCHAR(50),
  "card_number" VARCHAR(16),
  "CVV" VARCHAR(6),
  "expiration_date" VARCHAR(5),
  "creation_timestamp" timestamp DEFAULT CURRENT_TIMESTAMP,
  "amount" float NOT NULL
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

-- Aggiunta del constraint per il formato dell'email
ALTER TABLE "Users"
ADD CONSTRAINT "check_email_format"
CHECK ("email" ~* '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$');

ALTER TABLE "OrderItems" ADD FOREIGN KEY ("id_order") REFERENCES "Orders" ("id") ON DELETE CASCADE;
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
    ORDER BY "creation_timestamp" DESC
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

CREATE OR REPLACE FUNCTION order_drink(id_user_par int, id_drink int, quantity_par int)
RETURNS void AS $$
DECLARE
    order_id int;
    existing_quantity int;
BEGIN
    -- Controlla se esiste un ordine in corso (paid = false) per l'utente corrente
    SELECT "id" INTO order_id
    FROM "Orders"
    WHERE "id_user" = id_user_par
    AND "paid" = false
    LIMIT 1;
    
    IF order_id IS NULL THEN
        -- Se non esiste un ordine in corso, crea un nuovo ordine
        INSERT INTO "Orders" ("id_user") VALUES (id_user_par);
        SELECT lastval() INTO order_id;
    END IF;
    
    -- Controlla se esiste già un OrderItem con lo stesso id_item e collegato allo stesso Order
    SELECT "quantity" INTO existing_quantity
    FROM "OrderItems"
    WHERE "id_order" = order_id
    AND "id_item" = id_drink;
    
    IF FOUND THEN
        -- Aggiorna la quantità dell'OrderItem esistente
        UPDATE "OrderItems"
        SET "quantity" = existing_quantity + quantity_par
        WHERE "id_order" = order_id
        AND "id_item" = id_drink;
    ELSE
        -- Crea un nuovo OrderItem con l'id_item fornito e l'id_order ottenuto
        INSERT INTO "OrderItems" ("id_order", "id_item", "quantity") VALUES (order_id, id_drink, quantity_par);
    END IF;
    
    RETURN;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION delete_unpaid_order(id_user_par int)
RETURNS void AS $$
BEGIN
    -- Verifica se esistono ordini non pagati per l'utente specificato
    IF EXISTS (
        SELECT 1
        FROM "Orders"
        WHERE "id_user" = id_user_par
        AND "paid" = false
    ) THEN
        -- Elimina gli ordini non pagati dell'utente
        DELETE FROM "Orders"
        WHERE "id_user" = id_user_par
        AND "paid" = false;
    ELSE
        -- Se non ci sono ordini non pagati per l'utente, lancia un'eccezione
        RAISE EXCEPTION 'Non ci sono ordini non pagati per l''utente specificato.';
    END IF;

    RETURN;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION update_drink_quantity_from_unpaid_order(id_user_par int, id_drink_par int, new_quantity int)
RETURNS void AS $$
DECLARE
    order_id int;
    order_paid boolean;
BEGIN
    -- Controlla se l'OrderItem esiste e appartiene a un Order con paid = false
    SELECT "id_order", "paid" INTO order_id, order_paid
    FROM "OrderItems"
    INNER JOIN "Orders" ON "OrderItems"."id_order" = "Orders"."id"
    WHERE "OrderItems"."id_item" = id_drink_par
    AND "Orders"."id_user" = id_user_par
    AND "Orders"."paid" = false;

    IF order_id IS NULL THEN
        RAISE EXCEPTION 'L''OrderItem specificato non esiste o appartiene a un Order con paid = true.';
    END IF;

    -- Aggiorna la quantità dell'OrderItem
    UPDATE "OrderItems"
    SET "quantity" = new_quantity
    WHERE "id_order" = order_id
    AND "id_item" = id_drink_par;

    RETURN;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION delete_drink_from_unpaid_order(id_user_par int, id_drink_par int)
RETURNS void AS $$
DECLARE
    order_id int;
    order_paid boolean;
BEGIN
    -- Controlla se l'OrderItem esiste e appartiene a un Order con paid = false
    SELECT "id_order", "paid" INTO order_id, order_paid
    FROM "OrderItems"
    INNER JOIN "Orders" ON "OrderItems"."id_order" = "Orders"."id"
    WHERE "OrderItems"."id_item" = id_drink_par
    AND "Orders"."id_user" = id_user_par
    AND "Orders"."paid" = false;

    IF order_id IS NULL THEN
        RAISE EXCEPTION 'L''OrderItem specificato non esiste o appartiene a un Order con paid = true.';
    END IF;

    -- Cancella l'OrderItem
    DELETE FROM "OrderItems"
    WHERE "id_order" = order_id
    AND "id_item" = id_drink_par;

    RETURN;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION pay_unpaid_order(id_user_par int, card_holder_par VARCHAR(50), card_number_par VARCHAR(16), CVV_par VARCHAR(6), expiration_date_par VARCHAR(5), amount_par float)
RETURNS void AS $$
DECLARE
    total_amount float;
BEGIN
    -- Calcola il totale dell'importo dell'ordine
    SELECT SUM(oi.quantity * d.price)
    INTO total_amount
    FROM "Orders" o
    JOIN "OrderItems" oi ON o."id" = oi."id_order"
    JOIN "Drinks" d ON oi."id_item" = d."id"
    WHERE o."id_user" = id_user_par
    AND o."paid" = false;

    -- Verifica se esiste un ordine non pagato per l'utente specificato
    IF total_amount IS NOT NULL THEN
        -- Verifica se l'importo specificato è maggiore o uguale al totale dell'importo
        IF amount_par >= total_amount THEN
            -- Crea un nuovo pagamento per l'ordine non pagato
            INSERT INTO "Payments" ("id_order", "id_user", "card_holder", "card_number", "CVV", "expiration_date", "amount")
            SELECT "id", id_user_par, card_holder_par, card_number_par, CVV_par, expiration_date_par, amount_par
            FROM "Orders"
            WHERE "id_user" = id_user_par
            AND "paid" = false;

            -- Aggiorna lo stato di pagamento dell'ordine
            UPDATE "Orders"
            SET "paid" = true
            WHERE "id_user" = id_user_par
            AND "paid" = false;
        ELSE
            -- L'importo specificato è inferiore al totale dell'importo, lancia un'eccezione
            RAISE EXCEPTION 'L''importo specificato non è sufficiente per pagare l''ordine.';
        END IF;
    ELSE
        -- Se non ci sono ordini non pagati per l'utente, lancia un'eccezione
        RAISE EXCEPTION 'Non ci sono ordini non pagati per l''utente specificato.';
    END IF;

    RETURN;
END;
$$ LANGUAGE plpgsql;



-- Aggiungo indici di accesso per migliorare performance
CREATE INDEX idx_orders_user ON "Orders" ("id_user");
CREATE INDEX idx_order_items_order ON "OrderItems" ("id_order");
CREATE INDEX idx_order_items_item ON "OrderItems" ("id_item");

INSERT INTO "Users" ("email","password") VALUES
    ('alex@gmail.com','rbJPrGrTIacSw'), 
    ('mario@gmail.com','rbJPrGrTIacSw');
-- password di ogni user: 123
INSERT INTO "Drinks" ("name", "description", "image_url", "price") VALUES
    ('Negroni', 'Cocktail amaro e intenso','images/drinks/negroni.jpg', 2.5),
    ('Long Island', 'Cocktail forte e deciso','images/drinks/longisland.jpg', 3.0),
    ('Moscow Mule', 'Cocktail infuocato','images/drinks/moscow.jpg', 2.0),
    ('Cosmopolitan', 'Cocktail dolce','images/drinks/cosmopolitan.jpg', 2.75),
    ('Hugo', 'Molto forte','images/drinks/hugo.jpg', 3.5),
    ('Margarita', 'Molto forte','images/drinks/margarita.jpg', 4.5),
    ('Bianchi', 'Molto forte','images/drinks/bianchi.jpg', 2.5),
    ('Manhattan', 'Molto forte','images/drinks/manhattan.jpg', 3.5),
    ('Mojito', 'Molto forte','images/drinks/mojito.jpg', 6.5),
    ('Martini', 'Molto forte','images/drinks/martini.jpg', 3.5),
    ('Gin Tonic', 'Molto forte','images/drinks/gintonic.jpg', 3.0);

INSERT INTO "Drinks" ("name", "description", "image_url", "price","drink_type") VALUES
    ('Frullato di banana', 'Frullato gustoso','images/drinks/banana.jpg', 2.5,'frullato'),
    ('Frullato di more e lamponi', 'Frullato forte e deciso','images/drinks/more-lamp.jpg', 3.0,'frullato'),
    ('Frullato di fragole', 'Frullato forte e deciso','images/drinks/fragola.jpg', 2.0,'frullato'),
    ('Frullato di kiwi', 'Frullato gustoso','images/drinks/kiwi.jpg', 3.5,'frullato');