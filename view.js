document.getElementById("viewbtn").addEventListener("click", viewpdtdivfunction);

function viewpdtdivfunction() {
document.getElementById('viewpdtdiv').style.visibility = "visible";
document.getElementById('custTable').style.visibility = "hidden";
document.getElementById('viewbtn').style.display = "none";
document.getElementById('pdtclose').style.display = "block";
document.getElementById('custTable').style.height = "0px";

}

document.getElementById("pdtclose").addEventListener("click", closepdtdivfunction);

function closepdtdivfunction() {
document.getElementById('viewpdtdiv').style.visibility = "hidden";
document.getElementById('custTable').style.visibility = "visible";
document.getElementById('pdtclose').style.display = "none";
document.getElementById('viewbtn').style.display = "block";
document.getElementById('custTable').style.height = "1000px";

}


    var date_time
    function refreshTime() {
        const timeDisplay = document.getElementById("time");
        const dateString = new Date().toLocaleString('en-IN', {
            //weekday: 'short', // long, short, narrow
            day: 'numeric',// numeric, 2-digit
            month: 'numeric',
            year: 'numeric', // numeric, 2-digit
            hour: 'numeric', // numeric, 2-digit
            minute: 'numeric', // numeric, 2-digit
            second: 'numeric', // numeric, 2-digit
        })
        const formattedString = dateString.replace(", ", " - ");
        //timeDisplay.textContent = formattedString.toUpperCase();
        date_time = formattedString.toUpperCase();
    }
    setInterval(refreshTime, 1000);
    var productNameArray = []
    var product_CodeArray = []
    var ip_adderss = null;
    var productDct = {};
    const ipForm = document.getElementById("ipForm")
    const csvFile = document.getElementById("csvFile");
    const ipData = document.getElementById("ipadd");
    let button = document.getElementById("inpbtn")
    ipForm.addEventListener("submit", function (e) {
        e.preventDefault();
        const input = csvFile.files[0];
        const reader = new FileReader();
        ip_adderss = ipData.value;
        reader.onload = function (e) {
            const text = e.target.result;
        };
        reader.readAsText(input);
        reader.onload = function (e) {
            const text = e.target.result;
            const data = csvToArray(text);


            for (let i = 0; i <= data.length - 1; i++) {
                let k = data[i]
                product_CodeArray.push(k[[0]]);
                productNameArray.push(k[[1]]);
            };
            for (let j = 0; j <= product_CodeArray.length; j++) {
                if (typeof (productNameArray[j]) != 'undefined')
                    productDct[product_CodeArray[j]] = productNameArray[j]
            };
        };

        let ModalToClose = document.getElementById('select_id')
        ModalToClose.remove()

    });






    function csvToArray(str, delimiter = ",") {
        // slice from start of text to the first \n index
        // use split to create an array from string by delimiter
        const headers = str.slice(0, str.indexOf("\n")).split(delimiter);
        //console.log(headers)

        // slice from \n index + 1 to the end of the text
        // use split to create an array of each csv value row
        const rows = str.slice(str.indexOf("\n") + 1).split("\n");

        //console.log(rows)


        // Map the rows
        // split values from each row into an array
        // use headers.reduce to create an object
        // object properties derived from headers:values
        // the object passed as an element of the array
        const arr = rows.map(function (row) {
            const values = row.split(delimiter);
            const el = headers.reduce(function (objects) {
                object = [values[0], values[1]];

                return object;
            }, {});
            return el;
        });

        // return the array
        return arr;
    }

    var CustomerdataArray = [];
    var CartDataArray = [];
    var customerIdentified = false;
    var STATUS= 0;
    var STATE = 0;
    function GetCustomerData() {

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            STATE = this.readyState;
             STATUS= this.status;
           // console.log(this.readyState, this.status)
            if (this.readyState == 4 && this.status == 200) {
                const receivedCustomerData = this.responseText;
                CustomerdataArray = receivedCustomerData.split(" ");
                
                

                if (receivedCustomerData != "") {
                    customerIdentified = true
                }
                else {
                    customerIdentified = false
                }
             
            }
            showLoading()
            customerData()
        };


        
        xhttp.open("GET", `http://${ip_adderss}/rcvd`, true);
        xhttp.send(null);

    }


    function showLoading(){
        if(STATE == 2  && STATUS == 200){
         
            document.getElementById('status').style.color="#F1C40F";
        }
        if(STATE == 1 && STATUS == 0){
           
            document.getElementById('status').style.color="#CB4335";
        }
        if(STATE == 4 && STATUS ==200){
            document.getElementById('status').style.color = "#1E8449"
        };
    }



    function GetCartData() {

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                const receivedCartData = this.responseText;
                CartDataArray = receivedCartData.split(" ");
                //console.log(receivedCartData)
                cartData();
            }
        };
        xhttp.open("GET", `http://${ip_adderss}/cart`, true);
        xhttp.send(null);

    }





    var dataArray = [];

    function GetNewproductData() {

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                receivedProductData = this.responseText;
                dataArray = receivedProductData.split(" ");
                //console.log(receivedProductData);
                productData();
                GetCartData();

                initialCall = true;


            }
      
        };
        
        xhttp.open("GET", `http://${ip_adderss}/pdt`, true);
        xhttp.send(null);

    }
    // setInterval(GetNewproductData, 510);


    var initialCall = true;

    var customerId = '';
    var productId = '';
    var NewProductId = null;
    var price = '';
    var qty = '';
    var dataIdentifier = '';
    var pdtName = '';
    var newProduct = '';
    var newPrice = 0;
    var newQty = 0;
    var genericName = '';

    var productNameList = [];
    var productPriceList = [];
    var productQtyList = [];


    function productData() {
        customerId = dataArray[5];
        productId = dataArray[2];

        if (typeof (dataArray[4]) == 'undefined' || typeof (dataArray[3]) == 'undefined') {
            price == 0;
            qty == 0;

        }
        if (typeof (dataArray[4]) != 'undefined' && typeof (dataArray[3]) != 'undefined') {
            price = parseFloat(dataArray[4]);
            qty = parseFloat(dataArray[3]);

        }



        dataIdentifier = dataArray[0];
        pdtName = dataArray[1];

        if (typeof (pdtName) != 'undefined') {
            genericName = productDct[pdtName];

        };


        if (NewProductId == null) {
            Idcount = 1;

        }

        if (NewProductId != productId) {
            //console.log('productId : ',productId)
            // console.log('NewProductId : ',NewProductId)
            createProductElament();
            NewProductId = productId
        }
        console.log('ip_adderss : ', ip_adderss)


    };



    var customerIdenifier = '';
    var custUid = '';
    var custName = '';
    var custPhone = '';
    var customerCount = 1;
    var cusState = false;
    var cusStateCount = 0;

    function customerData() {

        customerIdenifier = CustomerdataArray[0];
        custUid = CustomerdataArray[1];

        if (typeof custUid != 'undefined') {
            custUid = custUid.toUpperCase();
            cusState = true;
            cusStateCount = cusStateCount + 1


        }
        else {
            cusState = false;
            cusStateCount = 0;
        }

        custName = CustomerdataArray[2];
        custPhone = CustomerdataArray[3];

        if (cusState == true && cusStateCount <= 1) {
            createCustomerNewElement();
        };


    };

    var cartIdentifier = '';
    var cartTotal = '';
    let FloatcartTotal = 0.00;
    var cartUid = '';
    var cart_state = false;
    var print_count = 0;

    function cartData() {


        cartIdentifier = CartDataArray[0];
        cartUid = CartDataArray[1];
        cartTotal = CartDataArray[2];


        if (typeof cartUid != 'undefined') {
            FloatcartTotal = parseFloat(cartTotal);
            //console.log(FloatcartTotal)
            cart_state = true;

            print_count = print_count + 1;

            var totalList = `Total : &#8377; ${Math.round(FloatcartTotal * 100) / 100}`;

            if (cart_state == true && print_count <= 1) {
                let total_ul = document.createElement('ul');
                total_ul.setAttribute('class', 'list-group')
                total_ul.setAttribute('style', 'list-style: none;')
                let modal_li = document.createElement('li');
                modal_li.setAttribute('class', 'list-group-item')
                modal_li.innerHTML = totalList;
                total_ul.appendChild(modal_li);
                document.getElementById('list_ul_new_pdt').appendChild(total_ul);
                newPrice = 0.00;
                newQty = 0.00;
                Idcount = Idcount + 100;


            }
        };
        if (typeof cartUid == 'undefined') {
            FloatcartTotal = 0.00;
            cart_state = false;
            print_count = 0;
        };
    };
    var Idcount = 0;
    function createCustomerNewElement() {
        let trow = document.createElement('tr');
        trow.setAttribute('scope', 'row');
        let row_2_data_1 = document.createElement('td');
        row_2_data_1.innerHTML = customerCount;
        let row_2_data_2 = document.createElement('td');
        row_2_data_2.innerHTML = custUid;
        let row_2_data_3 = document.createElement('td');
        row_2_data_3.innerHTML = custName;
        let row_2_data_4 = document.createElement('td');
        row_2_data_4.innerHTML = custPhone;
        let row_2_data_5 = document.createElement('td');
        row_2_data_5.innerHTML = date_time;
        trow.appendChild(row_2_data_1);
        trow.appendChild(row_2_data_2);
        trow.appendChild(row_2_data_3);
        trow.appendChild(row_2_data_4);
        trow.appendChild(row_2_data_5);
        //trow.appendChild(row_2_data_6);
        if (customerIdenifier == '$') {
            document.getElementById('Table_body').appendChild(trow);
            let list_data = document.createElement('li');
            list_data.innerHTML = custUid;
            customerCount = customerCount + 1;
        };
    };
    function createProductElament() {
        if (newProduct != pdtName && typeof (pdtName) != 'undefined') {
            newProduct = pdtName;
            createNewModal(qty, price, false);
            //console.log('new')
            console.log(Idcount)
        };
        if (newProduct == pdtName) {
            //console.log('old')
            console.log(Idcount)
            newPrice = price + newPrice
            newQty = qty + newQty
            createNewModal(newQty, newPrice, true);
            //console.log(dataArray)
        };
        Idcount = Idcount + 1;
    };
    function createNewModal(disp_quantity, disp_price, state) {

        if (state == true) {
            var idTodelete = `toDelete${Idcount}`
            let modal_Tr = document.createElement('tr');
            modal_Tr.setAttribute('id', idTodelete)
            const element = document.getElementById(`toDelete${Idcount - 1}`);
            console.log(element);
            if (element != null) {
                element.remove();
            }
            let modal_li_1 = document.createElement('td');
            modal_li_1.innerHTML = custName;
            let modal_li_2 = document.createElement('td');
            modal_li_2.innerHTML = pdtName;
            let modal_li_3 = document.createElement('td');
            modal_li_3.innerHTML = genericName;
            let modal_li_4 = document.createElement('td');
            modal_li_4.innerHTML = Math.round(disp_quantity * 100) / 100;
            let modal_li_5 = document.createElement('td');
            modal_li_5.innerHTML = Math.round(disp_price * 100) / 100;
            let modal_li_6 = document.createElement('td');
            modal_li_6.innerHTML = ""
            modal_Tr.appendChild(modal_li_1);
            modal_Tr.appendChild(modal_li_2);
            modal_Tr.appendChild(modal_li_3);
            modal_Tr.appendChild(modal_li_4);
            modal_Tr.appendChild(modal_li_5);
            modal_Tr.appendChild(modal_li_6);
            document.getElementById('list_ul_new_pdt').appendChild(modal_Tr);
            //console.log(state)
            //Idcount = Idcount+1;

        };

        if (state == false) {
            let modal_Tr1 = document.createElement('tr');
            modal_Tr1.setAttribute('id', idTodelete)
            let modal_li_1_1 = document.createElement('td');
            modal_li_1_1.innerHTML = custName;
            let modal_li_2_1 = document.createElement('td');
            modal_li_2_1.innerHTML = pdtName;
            let modal_li_3_1 = document.createElement('td');
            modal_li_3_1.innerHTML = genericName;
            let modal_li_4_1 = document.createElement('td');
            modal_li_4_1.innerHTML = Math.round(disp_quantity * 100) / 100;
            let modal_li_5_1 = document.createElement('td');
            modal_li_5_1.innerHTML = Math.round(disp_price * 100) / 100;
            modal_Tr1.appendChild(modal_li_1_1);
            modal_Tr1.appendChild(modal_li_2_1);
            modal_Tr1.appendChild(modal_li_3_1);
            modal_Tr1.appendChild(modal_li_4_1);
            modal_Tr1.appendChild(modal_li_5_1);
            document.getElementById('list_ul_new_pdt').appendChild(modal_Tr1);
            //console.log(state)

        };


    };
    var clo2 = false;
    function connect() {
        if (ip_adderss == null) {
            //console.log('not connected waiting for Ip')

            if (csvFile.value.length == 0) {
                button.disabled = true;
            } else {
                button.disabled = false;
            }
        }
        else {
            callAJAXfunctions();
        };
    };
    function callAJAXfunctions() {
        if (initialCall == true) {
            GetCustomerData();
            if (clo2 == false) {
               // alert(`Connected to ${ip_adderss}`);
                
                document.getElementById('custTable').style.visibility = "visible"
                document.getElementById('custTable').style.height = "1000px"
                document.getElementById('viewbtn').style.display = "block";
                clo2 = true;
            };
        };
        if (customerIdentified == true) {
            initialCall = false;
            GetNewproductData()
            //console.log('olIdcountd : ', Idcount)
        }
        if (customerIdentified == false) {
            initialCall = true;
        }
    }
    setInterval(connect, 500);