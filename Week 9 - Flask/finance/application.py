import os, datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_info = db.execute("SELECT username, cash FROM users WHERE id = :uid", uid = session["user_id"])
    user_cash = usd(user_info[0]['cash'])
    user_stocks = db.execute("SELECT symbol, shares FROM stocks JOIN users ON users.id = stocks.user_id WHERE users.id = :uid", uid = session["user_id"])
    stocks_value = 0


    # Calculate current price & total value
    for row in user_stocks:
        stock = lookup(row['symbol'])
        row['current_price'] = usd(int(stock['price']))
        row['total_value'] = int(stock['price']) * int(row['shares'])
        row['total_value_usd'] = usd(int(stock['price']) * int(row['shares']))
        stocks_value += row['total_value']

    total_balance = usd(user_info[0]['cash'] + stocks_value)
    return render_template("index.html", user_stocks=user_stocks, user_info=user_info, cash=user_cash, tb=total_balance, sv=usd(stocks_value))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)

        if not stock:
            return apology("Sorry, stock not found.")

        total_price = int(stock['price']) * int(shares)

        user_cash = db.execute("SELECT cash FROM users WHERE id = :uid", uid = session["user_id"])
        user_stocks = db.execute("SELECT symbol FROM stocks WHERE user_id = :uid AND symbol = :symb", uid = session["user_id"], symb = symbol)

        if int(shares) <= 0:
            return apology("Incorrect amount.")
        elif (user_cash[0]['cash'] - total_price) < 0:
            return apology("You don't have enough money.")
        elif not user_stocks:
            db.execute("UPDATE users SET cash = cash - :tp WHERE id = :uid", tp = total_price, uid = session["user_id"])
            db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (:uid, :symb, :shrs)", uid = session["user_id"], symb = symbol, shrs = shares)
            db.execute("INSERT INTO transactions (user_id, symbol, shares, date, action, amount) VALUES (:uid, :symb, :shrs, :now, :action, :ap)", uid = session["user_id"], symb = symbol, shrs = shares, now = datetime.datetime.now(), action = "Buy", ap = total_price)
            return redirect("/")
        else:
            db.execute("UPDATE users SET cash = cash - :tp WHERE id = :uid", tp = total_price, uid = session["user_id"])
            db.execute("UPDATE stocks SET shares = shares + :shrs WHERE user_id = :uid AND symbol = :symb", shrs = shares, uid = session["user_id"], symb = symbol)
            db.execute("INSERT INTO transactions (user_id, symbol, shares, date, action, amount) VALUES (:uid, :symb, :shrs, :now, :action, :ap)", uid = session["user_id"], symb = symbol, shrs = shares, now = datetime.datetime.now(), action = "Buy", ap = total_price)
            return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :uid", uid = session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        return render_template("quoted.html", stock=stock, price = usd(stock['price']))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        pw1 = request.form.get("password")
        pw2 = request.form.get("confirmation")
        user = db.execute("SELECT * FROM users WHERE username = :un", un = username)

        if user:
            return apology("Sorry, this username is already taken.")
        elif pw1 != pw2:
            return apology("Passwords must match.")
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (:un, :pw)", un = username, pw = generate_password_hash(pw1))
            return redirect("/")
    else:
        return render_template("register.html")

@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    """Update password"""
    if request.method == "GET":
        return render_template("account.html")
    else:
        current_pw = request.form.get("current_pw")
        new_pw = request.form.get("new_pw")
        confirm_pw = request.form.get("confirm_pw")

        user = db.execute("SELECT * FROM users WHERE id = :uid", uid = session["user_id"])

        if not check_password_hash(user[0]["hash"], current_pw):
            return apology("Incorrect current password.")
        elif new_pw != confirm_pw:
            return apology("Passwords must match.")
        else:
            db.execute("UPDATE users SET hash = :hash WHERE id = :uid", hash = generate_password_hash(new_pw), uid = session["user_id"])
            return redirect("/")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        user_stocks = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = :uid AND symbol = :symb", uid = session["user_id"], symb = symbol)
        stock = lookup(symbol)
        current_price = stock['price']
        sold_for = int(current_price) * int(shares)

        if int(shares) > user_stocks[0]['shares']:
            return apology("You do not own this number of shares.")
        elif int(shares) <= 0:
            return apology("Please input correct number of shares.")
        else:
            db.execute("UPDATE stocks SET shares = shares - :shrs WHERE user_id = :uid AND symbol = :symb", shrs = shares, uid = session["user_id"], symb = symbol)
            db.execute("UPDATE users SET cash = cash + :earnings WHERE id = :uid", earnings = sold_for, uid = session["user_id"])
            db.execute("INSERT INTO transactions (user_id, symbol, shares, action, date, amount) VALUES (:uid, :symb, :shrs, :action, :now, :amnt)", uid = session["user_id"], symb = symbol, shrs = shares, action = "Sell", now = datetime.datetime.now(), amnt = sold_for)
            db.execute("DELETE FROM stocks WHERE user_id = :uid AND shares = 0", uid = session["user_id"])

            return redirect("/")

    else:
        user_data = db.execute("SELECT symbol FROM stocks WHERE user_id = :uid", uid = session["user_id"])
        return render_template("sell.html", user_data=user_data)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
