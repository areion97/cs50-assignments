import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from finance.helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows_plus = db.execute("SELECT STOCK_SYMBOL, SUM(SHARES_NUM) AS shares_num FROM payment WHERE user_id = ? AND SIGN = '+' GROUP BY stock_symbol", session["user_id"])
    rows_minus = db.execute("SELECT STOCK_SYMBOL, SUM(SHARES_NUM) AS shares_num FROM payment WHERE user_id = ? AND SIGN = '-' GROUP BY stock_symbol", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    stock = 0
    row = list()
    for symbol in rows_plus.key():
        quote = lookup(symbol)
        stock = rows_plus[symbol]["shares_num"] - rows_minus[symbol]["shares_num"]
        if stock > 0:
            row[symbol] = { {"current_price" : quote["price"]}, {"current_value" : quote["price"]}}
        row["current_price"] = quote["price"]
        row["current_value"] = quote["price"] * (row["shares_num"])
        stock = stock + row["current_value"]


    total = stock + cash
    return render_template("index.html", rows=rows, total=total)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")


        if symbol == "" or len(lookup(symbol)) == 0:
            return apology("Symbol does not exist")

        if not shares.isdigit() or int(shares) <= 0:
            return apology("Shares must be a positive number")


        quote = lookup(symbol)
        totalAmount = quote["price"] * int(shares)

        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if rows[0]["cash"] < totalAmount:
            return apology("You don't have enough cash to afford the payment")

        db.execute("INSERT INTO payment (USER_ID,SIGN,STOCK_SYMBOL,SHARES_NUM,SHARES_PRICE,DATE_TIME) VALUES(?,?,?,?,?,?)", session["user_id"], "+", symbol, int(shares), totalAmount, datetime.now())
        db.execute("UPDATE users SET cash = ? WHERE id = ?", rows[0]["cash"]-totalAmount,session["user_id"])
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
        symbol = request.form.get("symbol")

        quote = lookup(symbol)
        quote["price_formatted"] = usd(quote["price"])

        return render_template("quoted.html", quote=quote)
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        pwd = request.form.get("password")
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if username == "" or password == "":
            return apology("Password or username empty")
        if len(rows) > 0:
            return apology("Username is already existing")
        if request.form.get("confirmation") != pwd:
            return apology("Password do not match")

        else:
            password_hash = generate_password_hash(pwd)

            db.execute("INSERT INTO users (username, hash) VALUES(?,?)", username, password_hash)
            return render_template("login.html")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if symbol == "" or len(lookup(symbol)) == 0:
            return apology("Symbol does not exist")

        if not shares.isdigit() or int(shares) <= 0:
            return apology("Shares must be a positive number")

        rows = db.execute("SELECT STOCK_SYMBOL, SUM(SHARES_NUM) AS shares_num FROM payment WHERE user_id = ? AND stock_sybol = ? GROUP BY stock_symbol", session["user_id"], symbol)

        if len(rows) == 0 or rows["shares_num"] < shares:
            apology("Error: You don't own enough shares")


        quote = lookup(symbol)
        totalAmount = quote["price"] * int(shares)

        db.execute("INSERT INTO payment (USER_ID,SIGN,STOCK_SYMBOL,SHARES_NUM,SHARES_PRICE,DATE_TIME) VALUES(?,?,?,?,?,?)", session["user_id"], "-", symbol, int(shares), totalAmount, datetime.now())
        db.execute("UPDATE users SET cash = ? WHERE id = ?", rows[0]["cash"]+totalAmount,session["user_id"])
        return redirect("/")

    else:
        return render_template("sell.html")
