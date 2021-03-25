from typing import Callable, Any, TypeVar, cast
from collections import OrderedDict


Function = TypeVar('Function', bound=Callable[..., Any])


def make_key(args, kwargs):  # type: ignore
    keys = args
    if kwargs:
        for item in kwargs.items():
            keys += item
    return keys


def cache(max_size: int) -> Callable[[Function], Function]:
    """
    Returns decorator, which stores result of function
    for `max_size` most recent function arguments.
    :param max_size: max amount of unique arguments to store values for
    :return: decorator, which wraps any function passed
    """
    def decorating_function(user_function: Function) -> Function:
        arg_cache: OrderedDict[Any, Any] = OrderedDict()
        full_cache = False

        def wrapper(*args, **kwargs):  # type: ignore
            nonlocal full_cache

            key = make_key(args, kwargs)
            link = arg_cache.get(key)

            if link is not None:
                return link

            result = user_function(*args, **kwargs)

            if full_cache:
                arg_cache.popitem(last=False)
                arg_cache[key] = result
            else:
                arg_cache[key] = result
                full_cache = (len(arg_cache) >= max_size)

            return result

        wrapper.__name__ = user_function.__name__
        wrapper.__doc__ = user_function.__doc__
        wrapper.__module__ = user_function.__module__
        return cast(Function, wrapper)

    return decorating_function
