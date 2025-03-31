let inventory = [];

async function loadInventory() {
try {
    const response = await fetch('/api/inventory', { method: 'GET' });
    if (!response.ok) {
    throw new Error("Failed to load inventory");
    }
    inventory = await response.json();
    console.log("Inventory loaded:", inventory);
} catch (error) {
    console.error("Error loading inventory:", error);
}
}

function getProductById(id) {
    return inventory.find(item => item.id === id);
}

    function getCart() {
    return fetch("/api/cart", {
        method: "GET"
    })
    .then(response => {
        if (!response.ok) {
        throw new Error("Failed to retrieve cart");
        }
        return response.json();
    })
    .then(data => {
        console.log("Cart Data:", data.cart);
        return data.cart;  //Server return cart json
    })
    .catch(error => {
        console.error("Error fetching cart:", error);
        return [];
    });
    }
  
  
  
  function setCartQuantity(productId, quantity = 1) {
    fetch("/api/cart/set", {
      method: "POST",
      credentials: "include",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({ id: productId, quantity: quantity })
    })
    .then(response => {
      if (!response.ok) {
        throw new Error("Failed to set item in cart");
      }
      return response.text();
    })
    .then(message => {
      console.log(message); //"Item added to cart."
    })
    .catch(error => console.error("Error:", error));
  }

  function getProductQuanity(productId){
    const cart = GetCart();
    const item = cart.find(p => p.id === productId);

    if(item){
        return item.quantity;
    } else {
        return 0;
    }
  }



  function calculateCartTotal() {
    const cart = getCart();
    return cart.reduce((sum, item) => {
      const product = getProductById(item.id);
      return sum + (product.price * item.quantity);
    }, 0);
  }
  