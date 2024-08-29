import { ChangeEvent } from "react";
import "./input.css";

export type InputChangeEvent = ChangeEvent<HTMLInputElement>

export type InputProps = {
  value: string,
  label: string,
  changeHandler: (e: InputChangeEvent) => void
};

const Input = ({value, label, changeHandler}: InputProps) => {
  return (
    <div className="input-group">
      <label>{label}</label>
      <input defaultValue={value} onChange={changeHandler}
             placeholder="Enter wifi password"/>
    </div>
  );
};

export default Input;
