function displayNamesTop(data){
    
    //empty old data
    $("#top3").empty()
    let counter = 1

    let popular = [2, 4, 6, 8, 1, 9]

    //insert all new data
    $.each(data, function(i, datum){
        if (popular.includes(datum["id"])) {
            let new_name = $("<div class='col-md-4 card-gutter'><div class='home-card'><a href='/view/" + datum["id"] + "'><img class='card-image' alt='" + datum["name"] + " Food Truck Image' src="+ datum["image"] +"><div class='card-header home'>"+ datum["name"] +"</div><div class='card-subheader-home'>Trending #"+ counter +"</div><div class='card-text'>Try their best selling "+ datum["popular_items"][0] + " and " + datum["popular_items"][1] +"!</div></a></div></div>")
            $("#top3").append(new_name)
            counter++
        }
    })
}

function confirmation_message(current_id){
    $("#feedback").empty()

    console.log("current_id: " + current_id)
    let button = $("<a href='/view/"+ current_id +"'><button type='button' id='view-button' class='btn btn-primary btn-search' data-dismiss='modal'>View</button></a>")
    
    $("#feedback").addClass("alert")
    $("#feedback").append('The food truck has been added!').append(button)
}

function get_and_save_name(data){
    let clear = true
    $(".alert-empty").removeClass("alert-empty");

    let expected_meal_price = $("#new_expected_meal_price").val().trim()
    if (expected_meal_price == "" || !/\d/.test(expected_meal_price)) {
        $("#new_expected_meal_price").val("").focus().addClass("alert-empty");
        clear = false
    }

    let similar_truck_ids = $("#new_similar_truck_ids").val().replace(/\s/g, "").trim().trim(",")
    if (similar_truck_ids == "" || !/^[\d,]+$/.test(similar_truck_ids)) {
        $("#new_similar_truck_ids").val("").focus().addClass("alert-empty");
        clear = false
    }

    let user_rating = $("#new_user_rating").val().trim()
    if (user_rating == "" || !/\d/.test(user_rating)) {
        $("#new_user_rating").val("").focus().addClass("alert-empty");
        clear = false
    }

    let popular_items = $("#new_popular_items").val().trim().trim(",")
    if (popular_items == "") {
        $("#new_popular_items").val("").focus().addClass("alert-empty");
        clear = false
    }

    let location = $("#new_location").val().trim()
    if (location == "") {
        $("#new_location").val("").focus().addClass("alert-empty");
        clear = false
    }

    let opening_hours = $("#new_opening_hours").val().trim()
    if (opening_hours == "" || !/\d/.test(opening_hours)) {
        $("#new_opening_hours").val("").focus().addClass("alert-empty");
        clear = false
    }

    let image = $("#new_image").val().trim()
    if (image == "") {
        $("#new_image").val("").focus().addClass("alert-empty");
        clear = false
    }

    let name = $("#new_name").val().trim()
    if (name == "") {
        $("#new_name").val("").addClass("alert-empty").focus();
        clear = false
    }

    if (!clear) {
        return
    }

    let data_to_save = {
        "name": name,
        "image": image,
        "opening_hours": opening_hours,
        "location": location,
        "popular_items": popular_items,
        "user_rating": user_rating,
        "similar_truck_ids": similar_truck_ids,
        "expected_meal_price": expected_meal_price
    }
    console.log(data_to_save)

    $.ajax({
        type: "POST",
        url: "add_name",                
        dataType : "json",
        contentType: "application/json",
        data: JSON.stringify(data_to_save),
        success: function(result){
            let all_data = result["data"]
            data = all_data
            $("#new_name").val("")
            $("#new_image").val("")
            $("#new_opening_hours").val("")
            $("#new_location").val("")
            $("#new_popular_items").val("")
            $("#new_user_rating").val("")
            $("#new_similar_truck_ids").val("")
            $("#new_expected_meal_price").val("")
            confirmation_message(data.length)
            $("#new_name").focus();
        },
        error: function(request, status, error){
            console.log("Error");
            console.log(request)
            console.log(status)
            console.log(error)
        }
    });
}

function update_name(){
    let id = window.location.pathname.split("/")[2]
    console.log(id);

    let clear = true
    $(".alert-empty").removeClass("alert-empty");

    let similar_truck_ids = $("#edit-similar-trucks").val().replace(/\s/g, "").trim().trim(",")
    if (similar_truck_ids == "" || !/^[\d,]+$/.test(similar_truck_ids)) {
        $("#edit-similar-trucks").val("").focus().addClass("alert-empty");
        clear = false
    }

    let popular_items = $("#edit-popular-items").val().trim().trim(",")
    if (popular_items == "") {
        $("#edit-popular-items").val("").focus().addClass("alert-empty");
        clear = false
    }

    let location = $("#edit-location").val().trim()
    if (location == "") {
        $("#edit-location").val("").focus().addClass("alert-empty");
        clear = false
    }

    let image = $("#edit-image").val().trim()
    if (image == "") {
        $("#edit-image").val("").focus().addClass("alert-empty");
        clear = false
    }

    let user_rating = $("#edit-rating").val().trim()
    if (user_rating == "" || !/\d/.test(user_rating) || user_rating<0 || user_rating>10) {
        $("#edit-rating").val("").focus().addClass("alert-empty");
        clear = false
    }

    let expected_meal_price = $("#edit-price").val().trim()
    if (expected_meal_price == "" || !/\d/.test(expected_meal_price) || expected_meal_price<0) {
        $("#edit-price").val("").focus().addClass("alert-empty");
        clear = false
    }

    let opening_hours = $("#edit-hours").val().trim()
    if (opening_hours == "" || !/\d/.test(opening_hours)) {
        $("#edit-hours").val("").focus().addClass("alert-empty");
        clear = false
    }

    let name = $("#edit-name").val().trim()
    if (name == "") {
        $("#edit-name").val("").addClass("alert-empty").focus();
        clear = false
    }

    if (!clear) {
        console.log("clear");
        return
    }

    let url = "/edit/" + id + "/update"
    console.log(name)

    $.ajax({
        type: "POST",
        url: url,                
        dataType: "json",
        contentType: "application/json",
        data: JSON.stringify({
            "id": id,
            "name": name,
            "image": image,
            "opening_hours": opening_hours,
            "location": location,
            "popular_items": popular_items,
            "user_rating": user_rating,
            "similar_truck_ids": similar_truck_ids,
            "expected_meal_price": expected_meal_price
        }),
        success: function(result){
            let all_data = result["data"]
            data = all_data
            window.location.href = "/view/" + id + "/"
            // add confirmation of changes saved 
        },
        error: function(request, status, error){
            console.log("Error");
            console.log(request)
            console.log(status)
            console.log(error)
        }
    });
}


$(document).ready(function(){
    $("#search-text").val("");

    if (window.location.pathname == "/"){
        displayNamesTop(data)
    }

    if (window.location.pathname == "/add") {
        let name_id = []
        $.each(data, function(i, datum){
            let name = datum["name"] + " - " + datum["id"]
            name_id.push(name)
        })

        $("#new_similar_truck_ids").autocomplete({
            source: name_id
        });
    }

    $("#submit_name").click(function(){                
        get_and_save_name(data)
    })

    $("#btn-save").click(function(){                
        update_name()
    })

    $( "#dialog" ).dialog({ autoOpen: false });
    $( "#btn-discard" ).click(function() {
    $( "#dialog" ).dialog( "open" );
    });

    $("#search").submit(function(event){
        event.preventDefault();
        let search = $("#search-text").val().trim()
        console.log("search result: ")
        console.log($("#search-text").val())
        if (search == "") {
            console.log("empty search")
            $("#search-text").val("").focus();
        } else {
            window.location.href = "/search/" + search
        }
    });
})