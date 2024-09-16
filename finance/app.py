import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    symbols = db.execute("SELECT * FROM purchases")
    for n in symbols:
        share = lookup(symbols[0]["symbol"])
        db.execute("UPDATE purchases SET price_per_share = ? WHERE symbol = ? AND user_id = ?",
                   share["price"], symbols[0]["symbol"], session["user_id"])
        db.execute("UPDATE purchases SET total_price = ? WHERE symbol = ? AND user_id = ?",
                   share["price"] * db.execute("SELECT number_of_shares FROM purchases WHERE symbol = ? and user_id = ?", symbols[0]["symbol"], session["user_id"])[0]["number_of_shares"], symbols[0]["symbol"], session["user_id"])
    balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    sumOfTotalStockPrice = db.execute(
        "SELECT SUM(total_price) FROM purchases WHERE user_id = ?", session["user_id"])
    balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    if sumOfTotalStockPrice[0]["SUM(total_price)"] == None:
        sumOfTotalStockPrice[0]["SUM(total_price)"] = 0
    return render_template("index.html", stocks=db.execute("SELECT * FROM purchases WHERE user_id = ?", session["user_id"]), sum=balance[0]["cash"] + sumOfTotalStockPrice[0]["SUM(total_price)"], user_balance=balance[0]["cash"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        elif not request.form.get("shares"):
            return apology("must provide shares", 400)
        elif not request.form.get("shares").isdigit() or float(request.form.get("shares")) % 1 != 0 or int(request.form.get("shares")) < 0:
            return apology("number of shares cannot be a negative number, letter, or decimal", 400)
        elif lookup(request.form.get("symbol")) == None:
            return apology("Symbol does not exist", 400)

        stock = lookup(request.form.get("symbol"))
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if user_cash[0]["cash"] < stock["price"] * int(request.form.get("shares")):
            return apology("You Don't Have Enough Cash", 400)

        check = db.execute("SELECT * FROM purchases WHERE user_id = ? AND symbol = ?",
                           session["user_id"], request.form.get("symbol"))
        if len(check) != 0:
            check = db.execute("SELECT number_of_shares FROM purchases WHERE user_id = ? AND symbol = ?",
                               session["user_id"], request.form.get("symbol"))
            db.execute("UPDATE purchases SET number_of_shares = ? WHERE user_id = ? AND symbol = ?",
                       check[0]["number_of_shares"] + int(request.form.get("shares")), session["user_id"], request.form.get("symbol"))
            db.execute("UPDATE purchases SET price_per_share = ? WHERE user_id = ? AND symbol = ?",
                       stock["price"], session["user_id"], request.form.get("symbol"))
            db.execute("UPDATE purchases SET total_price = ? WHERE user_id = ? AND symbol = ?", stock["price"] * (
                check[0]["number_of_shares"] + int(request.form.get("shares"))), session["user_id"], request.form.get("symbol"))
        else:
            db.execute("INSERT INTO purchases (user_id, symbol, number_of_shares, price_per_share, total_price) VALUES(?, ?, ?, ?, ?)",
                       session["user_id"], request.form.get("symbol"), request.form.get("shares"), stock["price"], stock["price"] * int(request.form.get("shares")))

        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash[0]["cash"] - (
            stock["price"] * int(request.form.get("shares"))), session["user_id"])
        db.execute("INSERT INTO history (user_id, purchase_type, symbol, number_of_shares, price_per_share, total_price) VALUES(?, ?, ?, ?, ?, ?)",
                   session["user_id"], "BUY", request.form.get("symbol"), request.form.get("shares"), stock["price"], stock["price"] * int(request.form.get("shares")))
        return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return render_template("history.html", history=db.execute("SELECT * FROM history WHERE user_id = ?", session["user_id"]))


@app.route("/money", methods=["GET", "POST"])
@login_required
def money():
    if request.method == "POST":
        if not request.form.get("money"):
            return apology("must provide amount", 400)
        elif not request.form.get("money").isdigit() or float(request.form.get("money")) % 1 != 0 or int(request.form.get("money")) < 0:
            return apology("amount cannot be a negative number, letter, or decimal", 400)
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   int(request.form.get("money")), session["user_id"])
        return redirect("/")
    return render_template("money.html")


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        if lookup(request.form.get("symbol")) == None:
            return apology("Symbol does not exist", 400)
        return render_template("quoted.html", quote=lookup(request.form.get("symbol")))
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("confirmation password is not the same with password")

        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get(
                "username"), generate_password_hash(request.form.get("password")))
        except:
            return apology("username already exist", 400)

        register = db.execute("SELECT id FROM users WHERE username = ?",
                              request.form.get("username"))
        session["user_id"] = register[0]["id"]
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        print(request.form.get("symbol"))
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        elif not request.form.get("shares"):
            return apology("must provide shares", 400)
        elif int(request.form.get("shares")) < 0:
            return apology("number of shares cannot be a negative number", 400)
        if int(request.form.get("shares")) > db.execute("SELECT number_of_shares FROM purchases WHERE user_id = ? and symbol = ?", session["user_id"], request.form.get("symbol"))[0]["number_of_shares"]:
            return apology("number of shares requested to sell exceeds that of owner's shares", 400)

        numberOfSharesLeft = db.execute("SELECT number_of_shares FROM purchases WHERE user_id = ? and symbol = ?",
                                        session["user_id"], request.form.get("symbol"))[0]["number_of_shares"] - int(request.form.get("shares"))
        share = lookup(request.form.get("symbol"))
        if numberOfSharesLeft == 0:
            db.execute("DELETE FROM purchases WHERE symbol = ? and user_id = ?",
                       request.form.get("symbol"), session["user_id"])
        else:
            db.execute("UPDATE purchases SET price_per_share = ? WHERE symbol = ? AND user_id = ?",
                       share["price"], request.form.get("symbol"), session["user_id"])
            db.execute("UPDATE purchases SET number_of_shares = ? WHERE symbol = ? AND user_id = ?",
                       numberOfSharesLeft, request.form.get("symbol"), session["user_id"])
            db.execute("UPDATE purchases SET total_price = ? WHERE symbol = ? AND user_id = ?",
                       numberOfSharesLeft * share["price"], request.form.get("symbol"), session["user_id"])
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   int(request.form.get("shares")) * share["price"], session["user_id"])

        db.execute("INSERT INTO history (user_id, purchase_type, symbol, number_of_shares, price_per_share, total_price) VALUES(?, ?, ?, ?, ?, ?)",
                   session["user_id"], "SELL", request.form.get("symbol"), int(request.form.get("shares")), share["price"], int(request.form.get("shares")) * share["price"])
        return redirect("/")

    symbolList = list()
    for symbol in db.execute("SELECT symbol FROM purchases"):
        symbolList.append(symbol["symbol"])
    return render_template("sell.html", symbols=symbolList)
