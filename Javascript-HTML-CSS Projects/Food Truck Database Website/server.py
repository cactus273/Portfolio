from flask import Flask
from flask import render_template
from flask import Response, request, jsonify, abort
app = Flask(__name__)


current_id = 10
data = [
    {
        "id": 1,
        "name": "Jan's Express Halal",
        "image": "https://spoonuniversity.com/cdn-cgi/image/width=1024,f=auto/https:/s3.amazonaws.com/secretsaucefiles/photos/images/000/213/046/original/20180907_122150.jpg?1536348403",
        "opening_hours": "7am - 7pm",
        "location": "The food cart is located on 120th and Broadway. It is right in front of \
Levien Gymnasium. From the Columbia gate on 116th and Broadway, simply walk in the uptown \
direction. From that direction, once you reach 120th street, Jan's express should be seen on your right.",
        "popular_items": ["Chicken over Rice", "Philly Cheesesteak", "Lamb Gyro"],
        "user_rating": 8,
        "similar_truck_ids": [2, 3, 9],
        "expected_meal_price": 8
    },
    {
        "id": 2,
        "name": "Hooda Halal 115th",
        "image": "https://s3-media0.fl.yelpcdn.com/bphoto/9vx1AZql_MriLNfwDj8ZgQ/348s.jpg",
        "opening_hours": "9am - 4am",
        "location": "The food cart is located on 115th and Broadway. It is right next to the \
Morton Williams supermarket. From the Columbia gate on 116th and Broadway, simply walk in the downtown \
direction. From that direction, once you reach 115th street, Houda Halal 115th should be seen on your right.",
        "popular_items": ["Chicken over Rice", "Lamb over Rice", "French Fries"],
        "user_rating": 9,
        "similar_truck_ids": [1, 3, 9],
        "expected_meal_price": 9
    },
    {
        "id": 3,
        "name": "Hooda Halal Barnard",
        "image": "https://fastly.4sqi.net/img/general/600x600/48476_EjeWSekl7kCA1tVfwnnCSx1UFeAvPsYYiTLtn8JoC9o.jpg",
        "opening_hours": "9am - 4am",
        "location": "The food cart is located on 118th and Broadway. It is right in front of \
Barnard College. Right across from the Columbia gate on 118th. \
From the direction of the Columbia 116th gate, Hooda Halal Barnard is on the left.",
        "popular_items": ["Chicken over Rice", "Lamb over Rice", "Shrimp and Fish over Rice"],
        "user_rating": 9,
        "similar_truck_ids": [1, 2, 9],
        "expected_meal_price": 9
    },
    {
        "id": 4,
        "name": "Bone Soup Hotpot",
        "image": "https://i.postimg.cc/mDwf6NDC/IMG-6703.png",
        "opening_hours": "11am - 7:30pm",
        "location": "The food cart is located on 118th and Broadway. It is near \
the Columbia gate on 118th. From the direction of the Columbia 116th gate, it is on the right and \
usually at the furthest end. Bone Soup Hotpot is easy to spot because of the truck's pink color.",
        "popular_items": ["Spicy Bone Soup Hotpot", "Pickled Vegetable Fish Soup", "Hot and Sour Beef Soup"],
        "user_rating": 10,
        "similar_truck_ids": [5, 6],
        "expected_meal_price": 12
    },
    {
        "id": 5,
        "name": "Miracle Thai Express",
        "image": "https://s3-media0.fl.yelpcdn.com/bphoto/ObDizKNUmgTTBGogjtv4Nw/o.jpg",
        "opening_hours": "11am - 7:30pm",
        "location": "The food cart is located on 117th and Broadway. It is near \
the Columbia gate on 118th. From the direction of the Columbia 116th gate, it is on the right.\
Miracle Thai Express is usually located before the crossing.",
        "popular_items": ["Drunken Noodles", "Pad See Ew", "Pad Thai"],
        "user_rating": 7,
        "similar_truck_ids": [4, 6],
        "expected_meal_price": 11
    },
    {
        "id": 6,
        "name": "Momo Delight",
        "image": "https://s3-media0.fl.yelpcdn.com/bphoto/GRX1O5nqab_QLgjlH8lfEQ/o.jpg",
        "opening_hours": "11am - 6pm",
        "location": "The food cart is located on 118th and Broadway. It is near \
the Columbia gate on 118th. From the direction of the Columbia 116th gate, it is on the right. Momo Delight usually the second furthest from the end.",
        "popular_items": ["Fried Momo", "Gravy Chicken over Rice", "Chow Mein"],
        "user_rating": 7,
        "similar_truck_ids": [4, 5],
        "expected_meal_price": 10
    },
    {
        "id": 7,
        "name": "Fogon",
        "image": "https://s3-media0.fl.yelpcdn.com/bphoto/vATBQTTPRkBfSoEm4CtDRg/348s.jpg",
        "opening_hours": "10:30am - 10pm",
        "location": "The food cart is located on 117th and Broadway. It is near \
the Columbia gate on 118th. From the direction of the Columbia 116th gate, it is on the right.\
Fogon is usually located before the crossing.",
        "popular_items": ["Lomo Saltado", "Quesadillas", "Fogon"],
        "user_rating": 7,
        "similar_truck_ids": [8, 10],
        "expected_meal_price": 10
    },
    {
        "id": 8,
        "name": "Souvlaki GR Gyro",
        "image": "https://foodtruckpages.com/wp-content/uploads/2016/08/Souvlaki-GR.jpg",
        "opening_hours": "10am - 9pm",
        "location": "The food cart is located on 116th and Broadway. It is near \
the Columbia gate on 116th. From the direction of the Columbia 116th gate, \
it is on the right side of the street. Souvlaki GR is right beside the subway station.",
        "popular_items": ["Signature Lamb Gyro", "Chicken Souvlaki", "Fries"],
        "user_rating": 6,
        "similar_truck_ids": [7, 10],
        "expected_meal_price": 10
    },
    {
        "id": 9,
        "name": "King Tut Halal",
        "image": "https://i.postimg.cc/28gJGXwg/IMG-7793.png",
        "opening_hours": "9am - 3am",
        "location": "The food cart is located on 116th and and Amsterdam. It is right across \
the Columbia gate on 116th and Amsterdam. From the direction of the Columbia 116th gate, it is straight ahead.\
King Tut Halal is located near the law building.",
        "popular_items": ["Lamb over Rice", "Chicken over Rice", "Chicken Gyro"],
        "user_rating": 6,
        "similar_truck_ids": [1, 2, 3],
        "expected_meal_price": 8
    },
    {
        "id": 10,
        "name": "Go For Fusion",
        "image": "https://i.postimg.cc/bJ6Vt6rC/IMG-7796.png",
        "opening_hours": "10am - 6pm",
        "location": "The food cart is located on 117th and Broadway. It is between \
the Columbia gate on 116th and 118th. From the direction of the Columbia 116th gate, it is on the right side of the street.\
Go For Fusion is usually located before the crossing.",
        "popular_items": ["Beef Curry", "Alo Gobi", "Chili Paneer"],
        "user_rating": 7,
        "similar_truck_ids": [7, 8],
        "expected_meal_price": 10
    },
]

# ROUTES

@app.route('/')
def hello_world():
   return render_template('home.html', data=data)

@app.route('/add')
def add():
    return render_template('add.html', data=data)

@app.route('/search/')
def search():
   return render_template('home.html', data=data)

@app.route('/view/<id>/')
def info(id=None):
    global data
    
    id = int(id)
    truck_id = data[id - 1]
    
    return render_template('info.html', data=data, truck_id=truck_id) 

@app.route('/edit/<id>/')
def edit(id=None):
    global data
    
    id = int(id)
    truck_id = data[id - 1]
    
    return render_template('edit.html', data=data, truck_id=truck_id) 

@app.route('/search/<search_text>')
def search_results(search_text=None):
    global data
    print(search_text)
    search_text_lower = search_text.lower()
    empty = True
     
    name_results = []
    menu_results = []
    location_results = []
    id_tracker = []
    for truck in data:
        if search_text_lower in truck["name"].lower():
            tup = (truck["name"], truck["id"], truck["popular_items"], truck["location"].split(), search_text, truck["image"])
            name_results.append(tup)
            id_tracker.append(truck["id"])
            empty = False
        
        if truck["id"] not in id_tracker:
            menu_items = []
            for search_item in truck["popular_items"]:
                if search_text_lower in search_item.lower():
                    menu_items.append(search_item)
                    tup = (truck["name"], truck["id"], truck["popular_items"], truck["location"].split(), menu_items, truck["image"])
                    empty = False
                    
                    if truck["id"] not in id_tracker:
                        menu_results.append(tup)
                        id_tracker.append(truck["id"])
                    else:
                        menu_results = menu_results[:-1]
                        menu_results.append(tup)  
            
                    
        if truck["id"] not in id_tracker:
            matches = []
            start_index = 0
            while True:
                index = truck["location"].lower().find(search_text_lower, start_index)
                if index == -1:
                    break
                
                if truck["location"][index:index + len(search_text_lower)] not in matches:
                    matches.append(truck["location"][index:index + len(search_text_lower)])
                start_index = index + 1
            
            if matches:        
                tup = (truck["name"], truck["id"], truck["popular_items"], truck["location"].split(), matches, truck["image"])
                location_results.append(tup)
                id_tracker.append(truck["id"])
                empty = False
      
    results_len = len(id_tracker)       
    return render_template('search.html', name_results=name_results, menu_results=menu_results, 
                           location_results=location_results, search_text=search_text, results_len=results_len,
                            empty=empty, data=data)


# AJAX FUNCTIONS

# ajax for people.js
@app.route('/add_name', methods=['GET', 'POST'])
def add_name():
    global data
    global current_id

    print(current_id)
    json_data = request.get_json()

    name = json_data["name"]
    image = json_data["image"]
    opening_hours = json_data["opening_hours"]
    location = json_data["location"]
    popular_items = json_data["popular_items"].split(",")
    for i in range(len(popular_items)):
        popular_items[i] = popular_items[i].strip()
    
    user_rating = int(json_data["user_rating"])
    similar_truck_ids = json_data["similar_truck_ids"].split(",")
    for i in range(len(similar_truck_ids)):
        similar_truck_ids[i] = int(similar_truck_ids[i].strip())
    
    expected_meal_price = int(json_data["expected_meal_price"])
    
    # add new entry to array with 
    # a new id and the name the user sent in JSON
    current_id += 1
    print(current_id)

    new_name_entry = {
        "id":  current_id,
        "name": name,
        "image": image,
        "opening_hours": opening_hours,
        "location": location,
        "popular_items": popular_items,
        "user_rating": user_rating,
        "similar_truck_ids": similar_truck_ids,
        "expected_meal_price": expected_meal_price
    }
    data.append(new_name_entry)

    #send back the WHOLE array of data, so the client can redisplay it
    return jsonify(data=data)
    
@app.route('/edit/<id>/update', methods=['GET', 'POST'])
def update(id=None):
    global data 
    
    json_data = request.get_json()
    id = int(id)
    name = json_data["name"]
    image = json_data["image"]
    opening_hours = json_data["opening_hours"]
    location = json_data["location"]
    popular_items = json_data["popular_items"].split(",")
    for i in range(len(popular_items)):
        popular_items[i] = popular_items[i].strip()
    
    user_rating = int(json_data["user_rating"])
    similar_truck_ids = json_data["similar_truck_ids"].split(",")
    for i in range(len(similar_truck_ids)):
        similar_truck_ids[i] = int(similar_truck_ids[i].strip())
    
    expected_meal_price = int(json_data["expected_meal_price"])
    
    updated_entry = {
        "id": id,
        "name": name,
        "image": image,
        "opening_hours": opening_hours,
        "location": location,
        "popular_items": popular_items,
        "user_rating": user_rating,
        "similar_truck_ids": similar_truck_ids,
        "expected_meal_price": expected_meal_price
    }
    
    data[id - 1] = updated_entry
        
    return jsonify(data=data)

if __name__ == '__main__':
   app.run(debug = True)
