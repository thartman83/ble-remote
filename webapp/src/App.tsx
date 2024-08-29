import { useState } from 'react'
import './App.css'
import Input, { InputChangeEvent } from './components/ui/input/input'
import Dropdown, { DropdownChangeEvent } from './components/ui/dropdown/dropdown';
import Button, { ButtonClickEvent, ButtonType } from './components/ui/button/button';

function App() {
  const [, setPasswd] = useState<string>("");
  const [, setWifi] = useState<string>("");

  const passwordChangeHandler = (e: InputChangeEvent) => {
    setPasswd(e.target.value);
  };

  const wifiChangeHandler = (e: DropdownChangeEvent) => {
    setWifi(e.target.value);
  };

  const scanClickHandler = (_: ButtonClickEvent) => {

  };

  const connectClickHandler = (_: ButtonClickEvent) => {

  };

  return (
    <>
      <h2>Wifi Setup</h2>
      <div className='wifi-group'>
        <Dropdown value="" label="Wifi Network" changeHandler={wifiChangeHandler} />
        <Input value="" label="Password" changeHandler={passwordChangeHandler} />
        <div className='button-group'>
          <Button label="Scan" buttonType={ButtonType.secondary}
                  clickHandler={scanClickHandler}/>
          <Button label="Connect" buttonType={ButtonType.primary}
                  clickHandler={connectClickHandler}/>
        </div>
      </div>
    </>
  )
}

export default App
